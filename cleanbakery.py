import pandas as pd

# Đọc file CSV gốc (đặt file CSV cùng thư mục với file .py)
df = pd.read_csv("bread_bakery.csv")

# Xóa dòng trống
df.dropna(inplace=True)

# Xóa dòng trùng
df.drop_duplicates(inplace=True)

# Chuẩn hóa tên sản phẩm (viết hoa chữ đầu, xóa khoảng trắng)
df['ItemName'] = df['ItemName'].str.strip().str.title()

# (Tuỳ chọn) Giữ lại các sản phẩm phổ biến
common_items = ['Bread', 'Coffee', 'Pastry', 'Cake', 'Sandwich', 'Tea']
df = df[df['ItemName'].isin(common_items)]

# Lưu file sạch sang TXT (dấu tab giữa các cột)
df.to_csv("bakery_cleaned.txt", sep='\t', index=False)

print("✅ Đã tạo file bakery_cleaned.txt sạch đẹp!")
