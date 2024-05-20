# 目录结构

## AIR
- AIR_F.ino为发射端代码
- AIR_J.ino为接收端代码
## ChuniMainBoardCode
- ChuniMainBoardCode.ino为主程序入口，主要修改的文件集中于AIR_RGB.h，keyboard_chuni.h之中
- 使用ATSAMD21G18A的串口1进行开发，通讯接口为Serial