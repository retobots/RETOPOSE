# **TÀI LIỆU YÊU CẦU NGHIỆP VỤ (BUSINESS REQUIREMENTS DOCUMENT \- BRD)**

## **1\. Tổng quan dự án**

### **1.1. Tên dự án**

Hệ thống thiết bị đeo nhắc nhở điều chỉnh tư thế ngồi học thông minh (Smart Posture Corrector).

### **1.2. Bối cảnh & Vấn đề cần giải quyết**

* Tình trạng ngồi sai tư thế (gù lưng, cúi sát mặt vào bàn, vẹo cột sống) ở lứa tuổi học sinh, sinh viên dẫn đến các bệnh học đường như cận thị, thoái hóa đốt sống cổ và cong vẹo cột sống.  
* Các thiết bị cảnh báo tư thế cơ bản hiện nay thường gặp lỗi **cảnh báo sai (false alarm)**: Thiết bị rung báo lỗi ngay cả khi người dùng thực hiện các cử động tự nhiên trong thời gian ngắn (cúi xuống nhặt bút, vươn vai, với lấy sách, uống nước). Điều này gây phiền toái, làm gián đoạn tập trung và khiến người dùng tắt thiết bị.

### **1.3. Mục tiêu dự án**

* Phát triển thiết bị phần cứng nhỏ gọn gắn lên lưng để theo dõi độ nghiêng cột sống theo thời gian thực.  
* Tích hợp thuật toán lọc nhiễu và phân biệt hành vi nhằm loại bỏ các tình huống sai tư thế giả, chỉ cảnh báo khi phát hiện tư thế xấu duy trì liên tục.  
* Đạt tỷ lệ phát hiện chính xác tư thế xấu \>90% và tỷ lệ cảnh báo giả \<5% trong môi trường học tập thực tế.

## **2\. Phạm vi dự án (Project Scope)**

### **2.1. Trong phạm vi (In-Scope)**

* Module phần cứng tích hợp vi điều khiển, cảm biến góc nghiêng/gia tốc và loa, đèn nhắc nhở  
* Thuật toán nhận diện và bộ lọc chống phát hiện sai tư thế giả (False-Positive Prevention Algorithm).  
* Cơ chế hiệu chuẩn (Calibration) tư thế chuẩn riêng biệt cho từng thể trạng người dùng.  
* Cảnh báo bằng động cơ rung hoặc tín hiệu LED/âm thanh.  
* Thiết kế vỏ hộp nhỏ gọn và phụ kiện đeo/kẹp thuận tiện, tối ưu chi phí.

### **2.2. Ngoài phạm vi (Out-of-Scope)**

* Phân tích chuyên sâu y khoa về cấu trúc xương (thiết bị chỉ đóng vai trò hỗ trợ nhắc nhở hành vi, không phải thiết bị chẩn đoán y tế).  
* Khả năng chống nước chuẩn ngâm sâu (chỉ cần chống ẩm mồ hôi cơ bản).

## **3\. Chân dung người dùng & Kịch bản sử dụng (User Personas & Use Cases)**

* **Người dùng chính:** Học sinh từ cấp 1 đến cấp 3, sinh viên, người ngồi làm việc bàn giấy liên tục trên 2 giờ/ngày.

### **Bảng kịch bản hành vi cần xử lý**

| Tình huống thực tế | Trạng thái góc nghiêng | Thời gian duy trì | Phản hồi mong muốn của hệ thống |
| :---- | :---- | :---- | :---- |
| **Ngồi học chuẩn** | Góc chuẩn $\pm {5}^{\circ }$ | Liên tục | Giữ im lặng, không rung/báo. |
| **Gù lưng, cúi sát sách** | Lệch chuẩn $>1{5}^{\circ }-2{0}^{\circ }$ | $>5$ giây liên tục | **Kích hoạt rung nhắc nhở** để người dùng thẳng lưng lại. |
| **Cúi nhặt bút rơi** | Lệch chuẩn lớn ($>4{5}^{\circ }$) | Rất ngắn ($1-3$ giây) | **Bỏ qua (Không rung)**, nhận diện là thao tác tạm thời. |
| **Vươn vai, ngả lưng thư giãn** | Lệch chuẩn về phía sau ($<-1{0}^{\circ }$) | 60 giây  | **Bỏ qua (Không rung)**. |
| **Đứng dậy đi lại/uống nước** | Gia tốc dịch chuyển lớn | Kéo dài | Tự động chuyển sang chế độ **Tạm dừng (Standby)**. |

## **4\. Yêu cầu chức năng chi tiết (Functional Requirements)**

### **4.1. Khởi động và Hiệu chuẩn tư thế chuẩn (Calibration)**

* **FR-01 (Hiệu chuẩn điểm 0):** Thiết bị phải có nút nhấn vật lý hoặc thao tác chạm để ghi nhận "Tư thế ngồi thẳng chuẩn" của người dùng khi bắt đầu phiên học.  
* **FR-02 (Lưu thông số tạm thời):** Góc pitch (nghiêng trước/sau) và roll (nghiêng trái/phải) tại thời điểm hiệu chuẩn được đặt làm mốc tham chiếu (${0}^{\circ },{0}^{\circ }$).  
* **FR-03 (Phản hồi xác nhận):** Thiết bị phát 2 nhịp rung ngắn để báo hiệu quá trình hiệu chuẩn thành công.

### **4.2. Cơ chế lọc chống cảnh báo giả (Anti-False-Positive Engine)**

Hệ thống bắt buộc phải áp dụng cơ chế lọc đa tầng kết hợp giữa **Ngưỡng góc**, **Cửa sổ thời gian (Time-Window Filter)** và **Phân tích chuyển động động (Dynamic Motion Analysis)**:

\[Đọc dữ liệu MPU6050\]  
        │  
        ▼  
\[Lọc nhiễu Kalman/Complementary Filter\]  
        │  
        ▼  
\[Kiểm tra góc nghiêng \> Ngưỡng sai (15°-20°)?\]  
   ├── KHÔNG ──\> Reset Timer \= 0; Trạng thái bình thường  
   └── CÓ  
        │  
        ▼  
\[Kiểm tra vận tốc góc / Gia tốc động\]  
   ├── Vận tốc góc cực cao (Cúi nhanh nhặt đồ / Xoay người) ──\> Đưa vào bộ đệm chờ, chưa rung  
   └── Vận tốc góc thấp (Tư thế tĩnh, ngồi gù thụ động)  
        │  
        ▼  
\[Bộ đếm thời gian (Debounce Timer) \> Ngưỡng trễ T (3 \- 5s)?\]  
   ├── Chưa đủ T (người dùng đã thẳng lưng lại) ──\> Hủy đếm, không rung  
   └── Duy trì liên tục \>= T ──\> KÍCH HOẠT RUNG CẢNH BÁO

* **FR-04 (Bộ đếm thời gian trễ \- Debounce Timer):**  
  * Khi góc lệch vượt ngưỡng cho phép (mặc định: lệch $>1{5}^{\circ }$ so với mốc chuẩn), bộ đếm thời gian nội bộ bắt đầu đếm.  
  * Nếu người dùng quay lại tư thế hợp lệ trong khoảng thời gian quy định (${T}_{delay}=3$ đến $5$ giây, có thể cấu hình), bộ đếm lập tức reset về 0 mà không kích hoạt rung cảnh báo.  
* **FR-05 (Phân biệt chuyển động động và chuyển động tĩnh):**  
  * Sử dụng con quay hồi chuyển (Gyroscope) để đo vận tốc góc:  
    * *Cúi nhặt bút:* Gia tốc biến thiên nhanh, vận tốc góc thay đổi đột ngột biên độ lớn trong thời gian $<2$ giây.  
    * *Tư thế ngồi sai thật sự:* Góc biến thiên từ từ và dừng lại ở vị trí sai với vận tốc góc xấp xỉ bằng 0 (trạng thái tĩnh).  
  * Thiết bị chỉ kích hoạt cảnh báo khi phát hiện trạng thái tĩnh sai tư thế kéo dài quá thời gian trễ.  
* **FR-06 (Phát hiện đứng dậy và chuyển động toàn thân):**  
  * Khi phát hiện chuỗi xung gia tốc đặc trưng của việc bước đi hoặc đứng dậy (qua trục Z và Y), hệ thống tự động tạm ngưng kiểm tra tư thế để tránh cảnh báo sai khi người dùng rời bàn học.

### **4.3. Cơ chế cảnh báo và phản hồi (Alerting & Feedback)**

* **FR-07 (Cảnh báo xúc giác \- Rung):**  
  * Sử dụng động cơ rung mini để nhắc nhở riêng tư, không gây ồn trong lớp học.  
  * Kiểu rung cảnh báo: Rung ngắt quãng nhẹ nhàng tăng dần; tự động ngắt ngay lập tức khi người dùng ngồi thẳng lưng trở lại đúng góc chuẩn.  
* **FR-08 (Cảnh báo quang học/âm thanh \- Tùy chọn):**  
  * Tích hợp đèn LED đa màu: Xanh lá (tư thế chuẩn/sạc đầy), Vàng (chờ hiệu chuẩn), Đỏ (cảnh báo ngồi sai/pin yếu).  
  * Còi chip (Buzzer) có switch bật/tắt để phụ huynh có thể bật khi trẻ tự học ở nhà.

### **4.4. Quản lý năng lượng (Power Management)**

* **FR-09 (Chế độ tiết kiệm pin \- Deep Sleep):** Tự động chuyển về trạng thái ngủ sâu nếu không phát hiện bất kỳ chuyển động nào trong vòng 3 phút liên tục.  
* **FR-10 (Đánh thức thông minh):** Tự động kích hoạt lại khi phát hiện có chuyển động nâng thiết bị hoặc qua nút bấm.

## **5\. Đặc tả phần cứng và cơ khí (Hardware & Mechanical Requirements)**

### **5.1. Thành phần điện tử cốt lõi**

* **Bộ vi điều khiển (MCU):** ESP32 (ưu tiên nếu mở rộng kết nối Bluetooth/App) hoặc Arduino Nano/Pro Mini (tối ưu kích thước nhỏ và giá thành thấp).  
* **Cảm biến IMU 6 bậc tự do:** MPU6050 (tích hợp 3 trục gia tốc kế Accelerometer và 3 trục con quay hồi chuyển Gyroscope, giao tiếp I2C).  
* **Cơ cấu chấp hành cảnh báo:** Động cơ rung mini dạng đồng xu (Coin Vibration Motor 3V), LED chỉ thị RGB hoặc còi buzzer nhỏ.  
* **Khối nguồn:** Pin Li-po dung lượng 300mAh – 500mAh (3.7V), module sạc bảo vệ TP4056 (cổng sạc USB-C), công tắc nguồn trượt vật lý.  
* **Bảng mạch:** Mạch in PCB 1 lớp rút gọn, dễ làm thủ công.

### **5.2. Thiết kế cơ khí & Vật liệu tiếp xúc**

* **Vỏ thiết bị (Enclosure):** Vỏ nhựa in 3D PETG nhỏ gọn, cạnh bo tròn, trọng lượng tổng thể $<45g$ tính cả pin.  
* **Cơ chế gắn trên người:** Kẹp lò xo kẹp vào cổ áo phía sau hoặc dây đai co giãn nhẹ đeo chữ Y qua lưng.  
* **Đệm tiếp xúc:** Lớp đệm silicon hoặc cao su mềm tiếp xúc với bề mặt da/áo nhằm chống trượt và tạo sự êm ái khi đeo lâu.

## **6\. Yêu cầu phi chức năng (Non-Functional Requirements)**

* **NFR-01 (Thời lượng pin):** Tối thiểu 8 tiếng hoạt động liên tục sau mỗi lần sạc đầy, đảm bảo đủ cho một ngày học tập.  
* **NFR-02 (Thời gian trễ xử lý):** Tần số lấy mẫu cảm biến đạt từ $50Hz$ đến $100Hz$, thuật toán xử lý dữ liệu và phản hồi rung trong vòng $<200ms$ sau khi chạm ngưỡng thời gian trễ ${T}_{delay}$.  
* **NFR-03 (An toàn điện & Nhiệt độ):** Nhiệt độ bề mặt vỏ khi sạc hoặc hoạt động liên tục không vượt quá 38 độ C; pin tích hợp mạch bảo vệ ngắt quá áp và ngắn mạch.

## **7\. Tiêu chí nghiệm thu (Acceptance Criteria)**

* \[ \] Thiết bị nhận diện được tư thế ngồi chuẩn sau khi bấm nút hiệu chuẩn với góc tham chiếu sai số $<{2}^{\circ }$.  
* \[ \] Kiểm thử kịch bản cúi nhặt đồ: Thực hiện cúi người $6{0}^{\circ }$ trong vòng 2 giây rồi ngồi thẳng lại; thiết bị **không được phép** rung cảnh báo (tỷ lệ thành công 10/10 lần thử nghiệm).  
* \[ \] Kiểm thử kịch bản gù lưng thật: Người dùng chủ động gù lưng góc $2{0}^{\circ }$; thiết bị phát rung chính xác sau khi duy trì tư thế đúng 3–5 giây.  
* \[ \] Kiểm thử ngắt cảnh báo: Ngay khi người dùng điều chỉnh thẳng lưng lại, rung phải ngắt trong vòng $<0.5$ giây.  
* \[ \] Thời lượng pin đo kiểm thực tế đạt $\geq 8$ giờ hoạt động liên tục.