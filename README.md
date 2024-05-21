# 目录结构

## AIR
- AIR_F.ino为发射端代码
- AIR_J.ino为接收端代码
    - 当采样数>850时候为没照到，被手挡住，触发按键，置0
    - 当采样数<850时候为照到，没有被手挡住，不触发按键，置1

## ChuniMainBoardCode
- ChuniMainBoardCode.ino为主程序入口，主要修改的文件集中于AIR_RGB.h，keyboard_chuni.h之中
- 使用ATSAMD21G18A的串口1进行开发，通讯接口为Serial

# TODO
- 重新测试是否因为代码写得不正确导致NKRO不正常，不能正常输出字符
    - 已经解决通过使用
- 地键出现抖动触发的问题
- 发射端的联调实际测试
    - 不知道是否出现问题
- 接收端以及发射端共同联调测试