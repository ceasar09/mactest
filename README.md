# mactest
mactest folder

智能家居云服务器，实现透传控制等功能

2015-11-16

服务器分为两侧

I、与APP交互端
1、APP采用hybrid H5方式进行控制
2、服务器采用nginx作为服务前端
3、静态页面负载到SQUID服务器
4、动态处理逻辑通过FastCGI或Upstream到上层C++服务器机群（对FastCGI不是十分了解，具体用到FastCGI与Upstream后续再定，对Nginx的FastCGI和Upstream模块不了解，不清楚这部分是否可以做负载均衡）
5、使用Redis或Memcached作为 正在服务服务器Index与用户关联关系数据库
6、服务器I与服务器II通过异步socket进行通信


II、与设备交互
1、前端一台C++负载均衡服务器（均衡逻辑待定），将设备ID与服务器使用Redis或Memcached进行保存
2、将设备请求发送到对应服务器上
3、与服务器I使用异步socket
