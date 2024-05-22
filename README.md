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
- 仍然存在于误触发的可能还需要继续调节val来修改
- 解决灯光问题，现在修改还NUM_LED会导致触发不正常
    - 灯光在键盘模式下相对正常
    - 还没有解决NUM_LED的问题
- 重新测试是否因为代码写得不正确导致NKRO不正常，不能正常输出字符
    - 已经解决通过使用
- 地键出现抖动触发的问题
    - 通过调节在keyboard_chuni.h中的val来解决
- 发射端的联调实际测试
    - 不知道是否出现问题
- 接收端以及发射端共同联调测试
