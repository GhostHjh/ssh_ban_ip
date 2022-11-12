# 一个简单的防ssh爆破程序

## 1. 命令列表

```shell
ban_ip -help   (查看帮助)
ban_ip -init   (在默认路径 重新/自动 生成配置文件)
ban_ip -config 配置文件        (指定配置路径的文件)
ban_ip -run    (使用默认路径的配置文件, 运行一次)
```

## 2. 支持的系统

* 默认支持 __Debian__ 和 __CentOS__
* 如果您的系统和有和上面系统一样的 __/var/log/auth.log__ 或者 __/var/log/secure__ , 以及 __/etc/hosts.dey__ 文件, 则可以直接使用

## 3. 注意事项

* 如果不希望自己写配置文件, 可以使用 `ban_ip -init` 来主动生成配置文件
* 配置文件的格式
  > ```ini
  > [log_file]
  > file_name = /var/log/auth.log
  > error_num = 5
  >
  > [hosts_file]
  > file_name = /etc/hosts.deny
  >
  > [setting]
  > int_time = 10
  > ```
  > * error_num 为 ssh 连接错误多少次禁止登入
  > * int_time 为 多久检测一次日志文件(单位为分钟)
  > * [setting] 块不是必要的, 不设置默认为 5
  >
