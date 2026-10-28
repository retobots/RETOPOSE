# HƯỚNG DẪN LÀM VIỆC VỚI GIT (GIT WORKFLOW GUIDELINES)

Tài liệu này quy định quy trình phối hợp và làm việc với Git cho các thành viên dự án RETOPOSE giữa 3 nhóm: **Firmware**, **Hardware**, và **Mechanical**.

---

## 1. Cấu Trúc Nhánh (Branch Strategy)

Repo được phân chia các nhánh chính như sau:

*   **`main`**: Nhánh chính (Production/Release), chứa phiên bản ổn định nhất của dự án.
*   **`firmware`**: Nhánh làm việc tập trung của Team Firmware.
*   **`hardware`**: Nhánh làm việc tập trung của Team Hardware.
*   **`mechanical`**: Nhánh làm việc tập trung của Team Mechanical.

---

## 2. Nguyên Tắc Cốt Lõi (Core Rules)

> [!CAUTION]
> **TUYỆT ĐỐI KHÔNG:**
> 1. **KHÔNG** push commit trực tiếp lên nhánh `main`.
> 2. **KHÔNG** sửa đổi hoặc can thiệp trực tiếp vào branch của team khác (ví dụ: Firmware không tự ý commit vào `hardware` hay `mechanical`).

*   Mỗi team có toàn quyền quản trị và làm việc thoải mái trên nhánh của team mình.
*   Đồng bộ / tích hợp vào `main` chỉ được thực hiện thông qua **Pull Request (PR) / Merge Request (MR)** và cần có sự phê duyệt (review) từ Tech Lead hoặc đại diện các bên liên quan.

---

## 3. Quy Trình Làm Việc Hàng Ngày (Workflow)

### Bước 1: Khởi tạo và chuyển sang nhánh của Team
Trước khi bắt đầu code/thiết kế, đảm bảo bạn đang ở đúng nhánh của team:

```bash
# Lấy dữ liệu mới nhất từ remote
git fetch origin

# Chuyển sang nhánh của team (chọn 1 trong 3 nhánh tương ứng)
git checkout firmware      # Dành cho team Firmware
# hoặc: git checkout hardware
# hoặc: git checkout mechanical

# Kéo code mới nhất về
git pull origin <tên-nhánh-của-team>
```

*(Khuyến nghị)*: Nếu làm tính năng riêng biệt hoặc thử nghiệm, nên tạo nhánh con từ nhánh của team:
```bash
git checkout -b feature/<tên-tính-năng> <tên-nhánh-của-team>
```

---

### Bước 2: Commit và Đẩy thay đổi

Sau khi hoàn thành công việc, commit rõ ràng và push lên nhánh của team mình:

```bash
git add .
git commit -m "[<TÊN_TEAM>] Mô tả ngắn gọn thay đổi"
# Ví dụ: git commit -m "[FW] Add SPI driver for IMU sensor"
# Ví dụ: git commit -m "[HW] Update power circuit schematic v1.1"
# Ví dụ: git commit -m "[MECH] Export 3D model for outer casing"

# Push lên nhánh của team
git push origin <tên-nhánh-của-team>
```

---

### Bước 3: Hợp nhất vào `main` (Release / Milestone)

Khi một mốc (milestone) hoặc phiên bản đạt yêu cầu kiểm thử:
1. Tạo **Pull Request (PR)** từ `<tên-nhánh-của-team>` vào `main`.
2. Ghi rõ changelog và các điểm ảnh hưởng liên team (nếu có).
3. Được Tech Lead / Reviewer duyệt trước khi merge vào `main`.

---

## 4. Quy Ước Đặt Tên Commit (Commit Convention)

Cú pháp: `[<TÊN_TEAM>] <Hành động>: <Nội dung>`
*   `[FW]` : Firmware
*   `[HW]` : Hardware
*   `[MECH]` : Mechanical

*Ví dụ:*
*   `[FW] feat: implement BLE peripheral advertising`
*   `[HW] fix: correct footprint of USB-C connector`
*   `[MECH] update: adjust mounting holes distance`

---

## 5. Lưu Ý Về File Lớn & Dữ Liệu Thiết Kế (Hardware & Mechanical)

*   **Gitignore**: Kiểm tra kỹ file `.gitignore` để tránh commit các file tạm, file backup từ CAD/Altium (như `*.bak`, `*.tmp`, logs, file cache...).
*   **Release Artifacts**: Ưu tiên commit file nguồn thiết kế gốc (PCB, schematic, CAD source) và file export chuẩn (STEP, Gerber, PDF, Hex/Bin) vào các thư mục quy định.
