# Auto Memory Scanner - Hướng dẫn sử dụng

## Giới thiệu

Tool này giúp bạn tự động scan memory của game JX1 mà **KHÔNG cần cài đặt Python hoặc Lua**. Chỉ cần Windows có sẵn PowerShell (Windows 10/11).

## Các file trong package

- **auto_scan.bat** - File chạy chính (double-click để chạy)
- **AutoScanMemory.ps1** - Script PowerShell chính
- **AUTO_SCAN_README.md** - File hướng dẫn này

## Cách sử dụng

### Bước 1: Chuẩn bị

1. Mở game JX1
2. Đảm bảo bạn nhìn thấy giá trị cần tìm (HP, MP, Gold, etc.)

### Bước 2: Chạy tool

**Cách 1: Chạy bằng Batch file (Đơn giản nhất)**
```
Double-click vào file: auto_scan.bat
```

**Cách 2: Chạy trực tiếp PowerShell**
```powershell
Right-click vào AutoScanMemory.ps1 -> Run with PowerShell
```

**Cách 3: Run as Administrator (Khuyến nghị)**
```
Right-click vào auto_scan.bat -> Run as Administrator
```

### Bước 3: Sử dụng menu

Sau khi chạy, bạn sẽ thấy menu:

```
============================================
   JX1 Auto Memory Scanner (PowerShell)
============================================

1) Scan for HP (Auto mode)
2) Scan for MP (Auto mode)
3) Scan for custom value
4) Scan for Gold/Money
5) Exit
```

### Ví dụ: Tìm địa chỉ HP

1. Chọn **1** (Scan for HP)
2. Nhập HP hiện tại của bạn (ví dụ: `1500`)
3. Đợi scan xong (có thể mất 1-2 phút)
4. Tool sẽ báo: "Found 5000 addresses"
5. **Thay đổi HP** trong game (bị đánh hoặc uống thuốc)
6. Nhập HP mới (ví dụ: `1200`)
7. Tool sẽ lọc lại: "Remaining: 50 addresses"
8. Lặp lại bước 5-6 cho đến khi còn <= 10 địa chỉ
9. Tool sẽ hiển thị địa chỉ tìm được và lưu vào file `hp_addresses.txt`

### Ví dụ: Tìm địa chỉ Gold

1. Chọn **4** (Scan for Gold/Money)
2. Nhập số Gold hiện tại (ví dụ: `50000`)
3. Thay đổi Gold (mua/bán đồ)
4. Nhập số Gold mới
5. Lặp lại cho đến khi tìm được địa chỉ

## Xử lý lỗi

### Lỗi: "Execution Policy"

Nếu gặp lỗi về Execution Policy, chạy lệnh sau trong PowerShell as Administrator:

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Lỗi: "Process not found"

- Đảm bảo game JX1 đang chạy
- Kiểm tra tên process trong Task Manager
- Nếu tên khác "JX1.exe", sửa trong script:
  ```powershell
  $ProcessName = "TenProcessThucTe"
  ```

### Lỗi: "Access Denied" hoặc scan không ra kết quả

- Chạy script as Administrator
- Tắt antivirus tạm thời
- Kiểm tra game có bật anti-cheat không

## Output Files

Sau khi scan xong, tool sẽ tạo các file:

- `hp_addresses.txt` - Địa chỉ HP tìm được
- `mp_addresses.txt` - Địa chỉ MP tìm được
- `custom_scan_results.txt` - Kết quả scan custom
- `memory_scan_results.txt` - Kết quả scan chung

Format: Mỗi dòng là một địa chỉ dạng hex:
```
0x12345678
0x23456789
0x34567890
```

## Tips & Tricks

### 1. Tối ưu scan tốc độ

- Chạy as Administrator để scan nhanh hơn
- Đóng các ứng dụng khác để giảm memory cần scan
- Tắt các tính năng game không cần thiết

### 2. Tăng độ chính xác

- **Thay đổi giá trị nhiều lần** để lọc chính xác
- Thay đổi với **số lượng lớn** (ví dụ HP từ 1500 -> 1000 thay vì 1500 -> 1490)
- Đợi một chút giữa các lần scan

### 3. Các loại giá trị có thể tìm

- **HP (Health)** - Máu
- **MP (Mana)** - Nội lực
- **Gold** - Tiền vàng
- **Experience** - Kinh nghiệm
- **Stamina** - Thể lực
- **Coordinates** - Tọa độ (X, Y)
- **Level** - Cấp độ
- **Item Quantity** - Số lượng vật phẩm

### 4. Sau khi tìm được địa chỉ

Bạn có thể:

1. **Sử dụng trong Cheat Engine**:
   - Mở Cheat Engine
   - Add address manually
   - Paste địa chỉ tìm được

2. **Sử dụng trong code C++**:
   ```cpp
   uintptr_t hpAddress = 0x12345678; // Địa chỉ tìm được
   int* pHP = (int*)hpAddress;
   *pHP = 9999; // Set HP = 9999
   ```

3. **Tạo pointer scan**:
   - Lưu địa chỉ tìm được
   - Restart game và tìm lại
   - So sánh để tìm pointer cố định

## Advanced Usage

### Tùy chỉnh process name

Mở file `AutoScanMemory.ps1` và sửa dòng:

```powershell
param(
    [string]$ProcessName = "TenGameBanMuon",  # Thay đổi ở đây
    [string]$ScanType = "int32"
)
```

### Scan với value type khác

Hiện tại hỗ trợ **Int32** (4 bytes). Để scan Float hoặc loại khác, cần modify hàm `ScanMemoryForInt32` trong script.

### Batch scan nhiều giá trị

Tạo file `batch_scan.txt` với nội dung:

```
1500
1450
1400
1350
```

Sau đó chạy script với input từ file.

## Troubleshooting

### Scan quá chậm

- Memory game quá lớn
- Giải pháp: Sử dụng Cheat Engine với Lua script thay vì PowerShell
- Hoặc chỉ scan trong region cụ thể

### Tìm được quá nhiều địa chỉ

- Thay đổi giá trị nhiều lần hơn
- Thay đổi với số lượng lớn hơn
- Check xem có phải giá trị display (hiển thị) khác với actual value không

### Không tìm được địa chỉ nào

- Giá trị có thể được mã hóa (encrypted)
- Giá trị có thể là Float chứ không phải Integer
- Giá trị có thể được lưu dưới dạng khác (string, bytes array)
- Thử scan "Unknown value" trong Cheat Engine để tìm

## So sánh với Cheat Engine

| Feature | Auto Scanner (PowerShell) | Cheat Engine |
|---------|---------------------------|--------------|
| Không cần cài đặt | ✅ Có | ❌ Cần cài |
| Tốc độ scan | ⚠️ Trung bình | ✅ Nhanh |
| Dễ sử dụng | ✅ Rất dễ | ⚠️ Cần học |
| Scan unknown value | ❌ Không | ✅ Có |
| Pointer scan | ❌ Không | ✅ Có |
| Tự động hóa | ✅ Có | ⚠️ Cần script |

## Kết hợp với Cheat Engine

Workflow đề xuất:

1. **Dùng Auto Scanner** để tìm địa chỉ nhanh
2. **Export** địa chỉ ra file
3. **Import** vào Cheat Engine để:
   - Xác minh lại
   - Tạo pointer scan
   - Tạo Cheat Table

## Support

Nếu gặp vấn đề:

1. Check Windows Event Log
2. Run script trong PowerShell ISE để debug
3. Kiểm tra antivirus/firewall
4. Thử chạy as Administrator

## License

Free to use for educational purposes only.

---

**Chúc bạn scan memory thành công! 🎮**
