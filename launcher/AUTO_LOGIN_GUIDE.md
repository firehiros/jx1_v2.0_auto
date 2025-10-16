# Auto-Login Feature Guide

## Tính năng tự động đăng nhập (Auto-Login)

Phiên bản 2.0 đã được bổ sung tính năng **tự động đăng nhập** thông qua process memory, giúp bạn quản lý nhiều tài khoản một cách dễ dàng và tự động.

---

## Các tính năng chính

### 1. **Auto-Login via Process Memory**
- Tự động đăng nhập vào game thông qua việc ghi trực tiếp username/password vào memory của process
- Không cần phải nhập thủ công mỗi lần khởi động
- Mật khẩu được mã hóa và lưu trữ an toàn

### 2. **Server Selection**
- Chọn cụm máy chủ (Server Cluster)
- Chọn máy chủ cụ thể trong cụm
- Tự động kết nối đến máy chủ đã chọn khi login

### 3. **Auto-Reconnect**
- Tự động phát hiện khi bị disconnect
- Tự động thử kết nối lại với số lần tối đa có thể cấu hình
- Theo dõi trạng thái kết nối liên tục

---

## Hướng dẫn sử dụng

### Bước 1: Thêm Account mới

1. Mở tab **Accounts** trong launcher
2. Click vào nút **➕ Add Account**
3. Một cửa sổ dialog sẽ hiện ra

### Bước 2: Điền thông tin Account

#### **Account Information**
- **Nickname**: Tên hiển thị cho tài khoản (để nhận biết)
- **Username**: Tên đăng nhập game
- **Password**: Mật khẩu game (sẽ được mã hóa tự động)

#### **Server Selection**
- **Server Cluster**: Chọn cụm máy chủ (Cụm 1, Cụm 2, Cụm 3...)
- **Server Name**: Chọn máy chủ cụ thể trong cụm

#### **Auto-Login Settings**
- ✅ **Enable Auto-Login**: Bật/tắt tự động đăng nhập
- ✅ **Enable Auto-Reconnect**: Bật/tắt tự động kết nối lại
- **Max Reconnect Attempts**: Số lần thử kết nối lại tối đa (1-10)

#### **Default Bot Mode** (Tùy chọn)
- Chọn chế độ bot mặc định khi khởi động account

### Bước 3: Khởi động Account

1. **Khởi động game thủ công** - Mở JX1.exe trước
2. Trong launcher, click nút **▶ Start** bên cạnh account muốn chạy
3. Launcher sẽ:
   - Tự động detect process game
   - Inject DLL vào game
   - **Tự động đăng nhập** với username/password đã lưu
   - Tự động chọn server đã cấu hình

---

## Cấu hình máy chủ

File `Models/ServerInfo.cs` chứa cấu hình các cụm máy chủ. Bạn có thể thêm/sửa máy chủ tại đây:

```csharp
new ServerCluster
{
    Name = "Cụm 1 - Trung Nguyên",
    Servers = new List<ServerInfo>
    {
        new ServerInfo { Name = "Tân Thủ 1", IpAddress = "103.x.x.1", Port = 5816 },
        new ServerInfo { Name = "Tân Thủ 2", IpAddress = "103.x.x.2", Port = 5816 },
        // ...
    }
}
```

---

## Cấu hình Memory Addresses

**QUAN TRỌNG**: Các địa chỉ memory trong `Services/AutoLoginService.cs` cần được cấu hình cho đúng với game:

```csharp
private static class MemoryAddresses
{
    public static IntPtr LoginStateAddress = new IntPtr(0x00000000);     // Địa chỉ trạng thái login
    public static IntPtr UsernameAddress = new IntPtr(0x00000000);       // Địa chỉ username field
    public static IntPtr PasswordAddress = new IntPtr(0x00000000);       // Địa chỉ password field
    public static IntPtr ServerAddressOffset = new IntPtr(0x00000000);   // Offset server selection
    public static IntPtr ConnectionStateAddress = new IntPtr(0x00000000); // Địa chỉ connection state
}
```

### Cách tìm Memory Addresses:

1. Sử dụng **Cheat Engine** hoặc **x64dbg**
2. Attach vào process game (JX1.exe)
3. Tìm địa chỉ các trường:
   - Username input field
   - Password input field
   - Login state flag
   - Connection state flag
4. Cập nhật vào `MemoryAddresses` class

---

## Auto-Reconnect Logic

### Cách hoạt động:

1. **Monitoring Thread** chạy mỗi 5 giây
2. Đọc trạng thái kết nối từ memory
3. Nếu phát hiện `Disconnected` hoặc `Error`:
   - Tăng biến đếm `ReconnectAttempts`
   - Đợi 5 giây
   - Thử đăng nhập lại
4. Nếu vượt quá `MaxReconnectAttempts`:
   - Dừng auto-reconnect
   - Đặt account về trạng thái `Error`

### Các trạng thái Login:

```csharp
enum LoginState
{
    Unknown = 0,         // Chưa xác định
    AtLoginScreen = 1,   // Đang ở màn hình login
    LoggingIn = 2,       // Đang đăng nhập
    LoggedIn = 3,        // Đã đăng nhập thành công
    InGame = 4,          // Đang trong game
    Disconnected = 5,    // Bị disconnect
    Error = 6            // Lỗi
}
```

---

## Bảo mật Password

### Mã hóa:
- Password được mã hóa bằng **XOR encryption** với key cố định
- Được lưu dưới dạng **Base64** trong config file
- **Khuyến nghị**: Thay thế bằng AES encryption trong môi trường production

### Cách hoạt động:
```csharp
// Khi lưu password
string encrypted = AutoLoginService.EncryptPassword(plainPassword);
account.PasswordHash = encrypted;

// Khi đăng nhập
string decrypted = DecryptPassword(account.PasswordHash);
WriteStringToMemory(processHandle, MemoryAddresses.PasswordAddress, decrypted);
```

---

## Multi-Account Management

### Quản lý nhiều tài khoản:

- **Start All**: Khởi động tất cả accounts đã enable (tối đa 5 đồng thời)
- **Stop All**: Dừng tất cả accounts đang chạy
- **Individual Control**: Bật/tắt từng account riêng lẻ

### Theo dõi thống kê:
- **Kills**: Tổng số quái đã giết
- **Items**: Tổng số items đã nhặt
- **Gold**: Tổng số vàng kiếm được
- **Runtime**: Tổng thời gian chạy của tất cả accounts

---

## Troubleshooting

### Auto-login không hoạt động?

1. **Kiểm tra Memory Addresses**:
   - Đảm bảo đã cấu hình đúng các địa chỉ memory
   - Game có thể update và thay đổi địa chỉ

2. **Kiểm tra Process**:
   - Game phải đang chạy trước khi Start account
   - Process name phải là `JX1.exe`

3. **Kiểm tra Login State**:
   - Debug để xem trạng thái login có đúng không
   - Có thể cần điều chỉnh timing (await Task.Delay)

### Auto-reconnect không hoạt động?

1. **Kiểm tra ConnectionStateAddress**:
   - Địa chỉ này phải trỏ đúng vào flag connection state

2. **Kiểm tra MaxReconnectAttempts**:
   - Tăng số lần retry nếu cần

3. **Kiểm tra Log**:
   - Xem Console output để debug
   - Events sẽ được log ra console

---

## API Events

### Subscribe to Auto-Login Events:

```csharp
_autoLoginService.LoginStarted += (sender, e) => {
    Console.WriteLine($"[{e.Account.Nickname}] Login started: {e.Message}");
};

_autoLoginService.LoginCompleted += (sender, e) => {
    Console.WriteLine($"[{e.Account.Nickname}] Login success!");
};

_autoLoginService.LoginFailed += (sender, e) => {
    Console.WriteLine($"[{e.Account.Nickname}] Login failed: {e.Message}");
};

_autoLoginService.Disconnected += (sender, e) => {
    Console.WriteLine($"[{e.Account.Nickname}] Disconnected");
};

_autoLoginService.Reconnecting += (sender, e) => {
    Console.WriteLine($"[{e.Account.Nickname}] Reconnecting... Attempt {e.Account.ReconnectAttempts}");
};
```

---

## Các file quan trọng

| File | Mô tả |
|------|-------|
| `Models/AccountProfile.cs` | Model chứa thông tin account |
| `Models/ServerInfo.cs` | Model chứa thông tin máy chủ |
| `Services/AutoLoginService.cs` | Service xử lý auto-login |
| `Services/AccountManager.cs` | Manager quản lý accounts |
| `Views/AddAccountDialog.xaml` | Dialog thêm account mới |
| `ViewModels/MainViewModel.cs` | ViewModel chính |

---

## Lưu ý bảo mật

⚠️ **QUAN TRỌNG**:

1. **Không commit password vào Git**
   - Config file chứa password đã được mã hóa
   - Nên thêm vào `.gitignore`

2. **Sử dụng encryption mạnh hơn**
   - XOR encryption chỉ là demo
   - Production nên dùng AES-256

3. **Bảo vệ config file**
   - Set permissions chỉ cho user hiện tại
   - Không share config file

---

## Roadmap

### Các tính năng sắp tới:

- [ ] UI để edit account đã tồn tại
- [ ] Export/Import account profiles
- [ ] Cloud sync accounts (tùy chọn)
- [ ] Thông báo Discord/Telegram khi disconnect
- [ ] Scheduled auto-start (tự động khởi động theo lịch)
- [ ] Account groups (nhóm accounts theo mục đích)

---

## Support

Nếu gặp vấn đề, vui lòng:
1. Check console output để xem logs
2. Verify memory addresses
3. Check game version compatibility

---

**Version**: 2.0.0
**Last Updated**: 2025-10-16
**Author**: JX1 Auto Team
