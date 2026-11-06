# BÁO CÁO THIẾT KẾ & GIA CÔNG PHẦN CỨNG PCB

**Dự án:** Thiết Bị Đeo Giám Sát Tư Thế Cột Sống Thông Minh (ESP32-C3 & MPU-6050)  
**Công nghệ chế tạo:** PCB 1 lớp thủ công (Gia công ủi nhiệt & ăn mòn hóa chất)  

---

## PHẦN 1: TỔNG QUAN HỆ THỐNG & DANH MỤC LINH KIỆN TRÊN EASYEDA

Hệ thống thiết bị đeo giám sát tư thế được thiết kế theo cấu trúc mạch lai (**Hybrid PCB**), nhằm cân bằng giữa mục tiêu tối ưu hóa diện tích, độ tin cậy cơ học và khả năng gia công thủ công tại xưởng lab/tại nhà.

- **Khối chức năng trung tâm & kết nối:** (ESP32-C3, GY-521, TP4056, giắc JST) sử dụng dạng module cắm xuyên lỗ (**Pin Header 2.54mm/THT**) giúp tăng độ bền cơ học và dễ dàng tháo lắp thay thế.
- **Linh kiện phụ trợ:** (Resistor, Diode, Transistor, Capacitor) ưu tiên chuẩn dán SMD (0805 / SOT-23 / SOD-123) bố trí ở mặt dưới (Bottom Layer) để thu nhỏ tối đa kích thước board mạch và tối ưu tuyến đi dây trên phíp đồng 1 lớp.

### Bảng Thống Kê Linh Kiện (BOM)

| STT | Ký hiệu | Tên linh kiện / Thông số kỹ thuật | Footprint EasyEDA | Mục đích sử dụng |
|:---:|:---|:---|:---|:---|
| 1 | U1 | ESP32-C3 SuperMini (RISC-V 160MHz) | HDR-TH_1x8P-P2.54 (2 hàng) | Vi điều khiển trung tâm, xử lý góc, BLE |
| 2 | U3 | Module sạc pin TP4056 Type-C | MODULE-TP4056_4P | Quản lý sạc pin Li-Po 1S, bảo vệ quá nạp/xả |
| 3 | U4 | Module cảm biến GY-521 (MPU-6050) | HDR-TH_1x8P-P2.54 | Thu thập góc nghiêng trục Roll/Pitch (I2C) |
| 4 | Q1, Q2 | Transistor NPN SS8050 (25V, 1.5A) | SOT-23-3 | Khóa bán dẫn công suất lái còi & motor rung |
| 5 | D1, D5 | Diode xung 1N4148 / 1N4007 | SOD-123 / SOD-123FL | Flyback diode dập xung ngược bảo vệ transistor |
| 6 | D3 | Diode Schottky SS14 / 1N5819 | SMA (DO-214AC) | Chống phân cực ngược đường nguồn cấp |
| 7 | BAT, VBRA, LED1, BT | Giắc cắm JST-XH 2.54mm 2 Pin | CONN-TH_2P-P2.54 | Kết nối ngoại vi: Pin Li-Po, Motor 0820, Còi |
| 8 | C2, C3, C1 | Tụ gốm 100nF (104) 50V | C0805 / CAP-TH_D6.3-P2.5 | Lọc nhiễu cao tần (Decoupling) chân nguồn |
| 9 | R3, R7 | Điện trở than 10k$\Omega$ 1/4W | AXIAL-0.3 | Trở kéo xuống (Pull-down) chân Base transistor |
| 10 | R4, R8 | Điện trở than 1k$\Omega$ 1/4W | AXIAL-0.3 | Trở định dòng kích bão hòa cực Base |
| 11 | R2 | Điện trở hạn dòng LED 200$\Omega$ 1/4W | AXIAL-0.3 | Bảo vệ đèn LED chỉ thị (dòng $\approx 6.5\text{ mA}$) |

---

## PHẦN 2: CHỨC NĂNG & NGUYÊN LÝ KỸ THUẬT TỪNG KHỐI MẠCH

### 1. Khối Nguồn & Phân Phối Năng Lượng (Power Management)
* **Bo sạc TP4056**: Nhận nguồn 5V DC từ cổng Type-C, quản lý chu kỳ sạc CC/CV cho pin Li-Po 1S. Tích hợp IC bảo vệ DW01A + FS8205A chống xả kiệt dưới 2.5V.
* **Chiến lược nguồn tách biệt**: Động cơ rung lấy nguồn trực tiếp từ điện áp pin (`VB+`) nhằm tránh hiện tượng sụt áp đường logic khi khởi động. Đường nguồn chính `VCC` qua Diode Schottky SS14 (`D3`) cấp cho vi điều khiển và còi báo.
* **Tụ hóa C3 (220µF/16V)**: Đóng vai trò bộ tích trữ động năng điện. Khi ESP32 phát sóng Wi-Fi/BLE hoặc động cơ khởi động kéo dòng tức thời, tụ phóng dòng bù áp, triệt tiêu hiện tượng sụt áp gây reset vi điều khiển (*Brownout Reset*).

### 2. Khối Vi Điều Khiển Trung Tâm (ESP32-C3 Core)
* **Hiệu năng**: Kiến trúc RISC-V 32-bit, xung nhịp 160MHz, tích hợp BLE 5.0 truyền dữ liệu tư thế không dây.
* **Lọc nhiễu C2 (100nF)**: Đặt sát chân nguồn 5V của ESP32-C3 SuperMini nhằm triệt tiêu sóng hài tần số cao từ anten RF và nhiễu chuyển mạch.
* **Quy hoạch chân I/O chuẩn hóa**:
  * `GPIO8` (SDA) & `GPIO9` (SCL): Giao tiếp Bus I2C với MPU-6050.
  * `GPIO10`: Xuất xung/logic kích Transistor lái Motor rung.
  * `GPIO4`: Đọc nút nhấn chức năng (`INPUT_PULLUP`).
  * `GPIO3`: Xuất tín hiệu PWM/Logic kích còi báo (Buzzer).
  * `GPIO2`: Điều khiển LED chỉ thị trạng thái (Active-LOW).

### 3. Khối Cảm Biến Góc Nghiêng (IMU GY-521 / MPU-6050)
* **Nguyên lý đo**: Kết hợp cảm biến gia tốc 3 trục (Accelerometer) và con quay hồi chuyển 3 trục (Gyroscope), giao tiếp Bus I2C tốc độ cao.
* **Tụ giải ghép C1 (100nF)**: Mắc song song giữa chân 3.3V và GND ngay sát pad module để lọc dao động biên độ điện áp, giúp bộ ADC 16-bit thu thập giá trị ổn định, giảm thiểu trôi góc (*Drift*).

### 4. Khối Điều Khiển Tải Động Cơ Rung (Vibration Haptic Driver)
* **Cơ chế đóng cắt**: Transistor SS8050 NPN chịu dòng liên tục 1.5A, chạy ở chế độ bão hòa (*Saturation Mode*). Dòng kích Base qua $R_4 = 1\text{ k}\Omega$ ở mức $\approx 2.6\text{ mA}$, bảo vệ an toàn cho chân GPIO vi điều khiển.
* **Điện trở xả tĩnh R3 (10k$\Omega$)**: Nối từ cực Base xuống GND giúp giữ điện áp kích ở mức 0V dứt khoát trong quá trình khởi động vi điều khiển (Bootloader) hoặc khi chân I/O ở trạng thái thả nổi (Hi-Z).
* **Diode dập xung D5 (Flyback - 1N4007W)**: Mắc song song ngược chiều với motor. Khi transistor ngắt đột ngột, năng lượng từ trường trong cuộn dây xả vòng qua diode theo công thức:
  $$V = -L \frac{di}{dt}$$
  giúp bảo vệ mối nối C-E của transistor Q2 không bị đánh thủng.

### 5. Khối Cảnh Báo Âm Thanh (Buzzer Driver)
* Cấu hình mạch tương tự khối motor rung: dùng transistor $Q_1$ (SS8050) kích từ chân `GPIO3` qua trở hạn dòng $R_8$ ($1\text{ k}\Omega$), trở xả $R_7$ ($10\text{ k}\Omega$) và diode dập xung $D_1$ (1N4148W), hỗ trợ điều biến tần số âm thanh qua PWM.

---

## PHẦN 3: NGUYÊN TẮC THIẾT KẾ PCB TRÊN EASYEDA

1. **Lỗ bắt ốc cơ khí:** Bố trí 4 góc bo mạch ($d = 3\text{ mm}$, $D = 5\text{ mm}$), khoảng cách tâm lỗ $64\text{ mm}$, bo tròn viền PCB.
2. **Bố trí Jack cắm / ESP32-C3 / TP4056:** Đặt sát viền mép mạch để thuận tiện cắm cáp Type-C và dây module ngoại vi.
3. **Module MPU-6050:** Bố trí chính giữa board để phản ánh trung thực trọng tâm và góc nghiêng của cơ thể.
4. **Phân lớp linh kiện:**
   * **Top Layer:** Linh kiện lớn, thao tác trực tiếp, cắm THT (Jack JST, MPU-6050, ESP32-C3, TP4056, tụ lọc lớn).
   * **Bottom Layer:** Linh kiện dán thụ động (Diode, Transistor, Điện trở, Tụ gốm nhỏ SMD).  
   *(Lưu ý: Flip linh kiện sang mặt Bottom để tránh đảo cực và nhầm chân khi hàn tay).*
5. **Định tuyến dây (Routing):**
   * Ưu tiên dây nguồn (Power Traces) $\rightarrow$ Dây tín hiệu (Signal Traces) $\rightarrow$ Đường chân linh kiện.
   * Khoảng cách cách điện (Clearance) tối thiểu giữa các nét là **$16\text{ mil}$** để chống nhiễu và hạn chế chập dính thiếc hàn.
   * Sử dụng Jumper / Via xuyên lỗ khi cần cầu nối nhảy dây lên mặt Top Layer.
6. **Mặt đồng tiếp địa (Copper Area):** Đổ đồng toàn bộ mặt dưới làm lớp GND chung, không cần chạy dây GND riêng lẻ.

---

## PHẦN 4: TIÊU CHUẨN ĐI DÂY (DESIGN RULE) CHO MẠCH 1 LỚP THỦ CÔNG

Do gia công thủ công không có lớp phủ bảo vệ (Solder Mask), các thông số định tuyến tuân theo chuẩn **IPC-2221** và hiệu chỉnh cho phương pháp ăn mòn tại xưởng:

| Tên Net List | Nhóm Rule | Dòng max ($I$) | Track Width | Clearance | Via Pad / Hole | Giải thích kỹ thuật |
|:---|:---|:---:|:---:|:---:|:---:|:---|
| `+5V`, `VCC`, `VB+` | Default | $\approx 0.32\text{ A}$ | 40 - 60 mil | 16 mil | 1.40 / 0.80 mm | Đường nguồn chính, chống sụt áp tức thời |
| `GND` | GND | $\approx 0.32\text{ A}$ | 20 mil | 16 mil | 1.40 / 0.80 mm | Đường mass chung toàn mạch kết hợp phủ mass |
| `VBRA` | Default | $\approx 0.09\text{ A}$ | 20 mil | 16 mil | 1.20 / 0.60 mm | Đường cấp cực âm Motor rung từ cực C của Q2 |
| `BZ` | Default | $\approx 0.03\text{ A}$ | 30 mil | 16 mil | 1.20 / 0.60 mm | Dây tải âm còi Buzzer từ cực C của Q1 |
| `+3.3V` | Default | $\approx 0.01\text{ A}$ | 20 mil | 16 mil | 1.20 / 0.60 mm | Nguồn áp ổn định cấp cho cảm biến MPU-6050 |
| `LED1` | Default | $\approx 0.007\text{ A}$ | 20 mil | 16 mil | 1.20 / 0.60 mm | Tín hiệu điều khiển LED chỉ thị |
| `SDA`, `SCL` | Default | $< 0.001\text{ A}$ | 20 mil | 16 mil | 1.20 / 0.60 mm | Bus I2C, kích thước mảnh dễ luồn qua chân pin |
| `VIBRATOR`, `BUZZER` | Default | $\approx 0.003\text{ A}$ | 20 mil | 16 mil | 1.20 / 0.60 mm | Tín hiệu logic kích cực Base transistor |
| `BTN`, `R3_2`, `R8_2`... | Default | $< 0.001\text{ A}$ | 20 mil | 16 mil | 1.20 / 0.60 mm | Chân tín hiệu phụ trợ & mạch phân áp |

> **Quy định Copper Pour (Bottom):** Đặt Clearance tối thiểu $16\text{ mil}$ để rãnh cách ly giữa pad hàn và mặt đồng đủ rộng, triệt tiêu nguy cơ lem thiếc gây chập mass khi hàn thủ công.

---

## PHẦN 5: QUY TRÌNH GIA CÔNG MẠCH THỦ CÔNG & LẮP RÁP

```
[Xuất file In] ➔ [Ủi nhiệt / Dung môi] ➔ [Ăn mòn hóa chất] ➔ [Khoan lỗ] ➔ [Hàn & Kiểm tra Dry-run]
```

### Giai đoạn 1: Chuẩn Bị & Xuất Bản In (Export)
* Xuất bản vẽ PDF tỷ lệ 1:1 từ giao diện EasyEDA PCB (mặt Bottom Layer dạng Mirror/Invert tùy loại giấy chuyển nhiệt).
* Dụng cụ: Cồn $90^\circ$, acetone, bột chà bóng, phíp đồng 1 lớp, dung dịch ăn mòn ($FeCl_3$ hoặc muối nhôm / $H_2O_2$).

### Giai đoạn 2: Gia Công Phíp Đồng & Ăn Mòn Hóa Chất
1. **Làm sạch phíp:** Cắt phíp đồng theo kích thước thiết kế ($64 \times 64\text{ mm}$). Dùng giấy ráp mịn chà sạch lớp oxit bề mặt, lau sạch lại bằng cồn $90^\circ$, sấy khô tuyệt đối (tránh chạm tay trần vào mặt đồng).
2. **Chuyển mực in lên phíp:** Áp mặt giấy in có mực lên bề mặt đồng. Sử dụng nhiệt (bàn ủi) hoặc hỗn hợp cồn/acetone tỉ lệ $1:3$ để cố định lớp mực lên đồng.
3. **Bóc giấy & Dặm nét:** Ngâm phíp vào nước ấm 10 phút cho rã giấy, bóc tách nhẹ nhàng. Soát lại đường mạch, dùng bút lông dầu dặm lại các nét bị đứt hoặc rỗ.
4. **Ăn mòn hóa chất:** Thả phíp vào dung dịch ăn mòn pha nước ấm ($50^\circ\text{C} - 60^\circ\text{C}$), lắc nhẹ liên tục. Nhờ diện tích phủ mass lớn, thời gian ăn mòn chỉ mất khoảng 15–20 phút.
5. **Làm sạch mực:** Rửa phíp dưới vòi nước, dùng bông thấm cồn/acetone lau sạch lớp mực bảo vệ để lộ đường đồng.

### Giai đoạn 3: Khoan Lỗ Chân Linh Kiện
* Dùng mũi khoan $0.7\text{ mm} - 0.8\text{ mm}$ cho các chân điện trở, diode, transistor THT.
* Dùng mũi khoan $1.0\text{ mm}$ cho chân header cắm module và giắc JST-XH.
* Dùng mũi khoan $3.0\text{ mm}$ cho 4 lỗ cố định board mạch ở 4 góc.

### Giai đoạn 4: Quy Trình Hàn & Kiểm Tra (Dry-Run)
1. **Thứ tự hàn:**
   * Linh kiện thấp/SMD trước: Điện trở, Diode, Tụ gốm.
   * Linh kiện bán dẫn & phụ trợ: Transistor, Tụ hóa, Giắc cắm JST.
   * Hàng rào cắm (Female Pin Header) cho module ESP32-C3 và GY-521. *(Lưu ý: Không hàn chết trực tiếp module lên phíp đồng).*
2. **Kiểm tra thông mạch (Dry Run):**
   * Sử dụng thang đo Ohm/Buzzer trên VOM kiểm tra giữa `VCC` và `GND` (bảo đảm **tuyệt đối không bị chập mass**).
   * Đo kiểm tra thông mạch từ chân `OUT-` của bo sạc TP4056 đến toàn bộ các điểm GND trên bo.
3. **Cấp nguồn & Nạp chương trình:**
   * Cắm pin qua giắc `BAT`, đo nguồn $5\text{V}$ và $3.3\text{V}$ tại các chân header rỗng.
   * Khi điện áp đã chuẩn xác và ổn định, tiến hành cắm module ESP32-C3 và GY-521 vào đúng chiều để nạp code và vận hành thử nghiệm.