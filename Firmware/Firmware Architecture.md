PHẦN 1: KIẾN TRÚC HỆ THỐNG

Hệ thống được tổ chức theo mô hình phân tầng từ trên xuống dưới:  

Tầng Điều Phối (Orchestrator):  
Đóng vai trò là khối trung tâm điều khiển toàn bộ dòng thực thi của ứng dụng.  
Được hiện thực hóa qua các file: posture_analyzer.c và main.cpp.  
Thực hiện việc điều phối hai tầng bên dưới: Tầng Logic Nghiệp Vụ và Tầng Đồng Hành Phần Cứng. 

Tầng Logic Nghiệp Vụ (Logic):  
Đảm nhiệm việc nhận diện và phân tích các trạng thái/hành vi tư thế cụ thể của người sử dụng.  

Gồm 5 module mã nguồn tương ứng:  
posture_slouch.c: Nhận diện trạng thái gù lưng.  
posture_drop_pen.c: Nhận diện hành vi cúi tạm thời (nhặt đồ/nhặt bút).  

posture_stretch.c: Nhận diện hành vi vươn vai thư giãn.  
posture_walk.c: Nhận diện trạng thái vận động, đi lại.  
posture_normal.c: Nhận diện tư thế chuẩn, ổn định.  

Tầng Đồng Hành Phần Cứng (Hardware Drivers):  
Giao tiếp trực tiếp và điều khiển các thiết bị ngoại vi.  
Gồm 4 trình điều khiển driver:  
imu_sensor.c (MPU6050): Đọc tín hiệu góc nghiêng và gia tốc từ cảm biến chuyển động MPU6050.  
vibrator.c (Motor Rung): Điều khiển mô-tơ rung cảnh báo xúc giác.  
speaker.c (Động Loa): Điều khiển còi/loa phát cảnh báo âm thanh.  
led_indicator.c (Đèn LED): Điều khiển đèn LED hiển thị trạng thái hoạt động.  


PHẦN 2: QUY TRÌNH HOẠT ĐỘNG

Quy trình vận hành gồm giai đoạn cấu hình ban đầu và một vòng lặp tuần hoàn khép kín:  
0. Khởi tạo (SETUP) 
Chạy một lần duy nhất khi hệ thống được cấp nguồn/khởi động.  
Thực hiện: Lưu các thông số mốc thời gian và khai báo các biến toàn cục cần thiết.  
Sau khi khởi tạo xong, luồng điều khiển chuyển tiếp vào VÒNG LẶP CHÍNH (Loop Tick).  

Vòng Lặp Chính (Loop Tick) 
Vòng lặp được thực thi liên tục qua các bước tuần tự:  

Bước 1 - Đọc dữ liệu: Hệ thống thực hiện đọc dữ liệu từ cảm biến IMU đúng 1 lần duy nhất trong chu kỳ.  

Bước 2 - Đưa dữ liệu qua BỘ LỌC TÌNH HUỐNG (xét lần lượt theo thứ tự ƯU TIÊN):  
Ưu tiên 1 - Chuyển động toàn thân (Đi lại)? 

CÓ: Chuyển hệ thống sang chế độ STANDBY, tắt rung →\rightarrow→ Kết thúc chu kỳ.  
KHÔNG: Chuyển xuống xét Ưu tiên 2.  

Ưu tiên 2 - Hành vi tạm thời (Cúi nhặt bút)? 

CÓ: Reset Timer đếm thời gian gù lưng, giữ trạng thái không rung (tránh cảnh báo sai) →\rightarrow→ Kết thúc chu kỳ.  
KHÔNG: Chuyển xuống xét điều kiện tiếp theo.  

Ưu tiên 3 - Gù lưng kéo dài (<15∘<15^\circ<15∘, >5>5>5s)? (Lưu ý: trong ảnh ghi nhầm nhãn thành Ưu tiên 4 và điều kiện ghi nhầm dấu thành <15°, chuẩn logic là >15°): 

CÓ: Kích hoạt MOTOR RUNG để cảnh báo →\rightarrow→ Kết thúc chu kỳ.  
KHÔNG: Chuyển xuống xét điều kiện tiếp theo.  

Ưu tiên 4 - Ngả lưng vươn vai (<−10∘<-10^\circ<−10∘, <60<60<60s)?: 

CÓ: Tắt rung (cho phép người dùng thư giãn trong thời gian giới hạn) →\rightarrow→ Kết thúc chu kỳ.  
KHÔNG: Chuyển xuống xét Ưu tiên 5.  

Ưu tiên 5 - Tư thế chuẩn (±5∘\pm 5^\circ±5∘)?: 

CÓ: Reset toàn bộ các Timer, tắt rung →\rightarrow→ Kết thúc chu kỳ.  
Bước 3 - Cập nhật trạng thái: Hệ thống cập nhật trạng thái hoạt động hiện tại và chờ chu kỳ tiếp theo trong khoảng thời gian từ 10ms đến 20ms trước khi quay trở lại đầu vòng lặp chính.  
