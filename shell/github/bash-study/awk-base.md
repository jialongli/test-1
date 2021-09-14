
# AWK 基本操作

## 输入数据

```text
<  写入
> 输出
```

比如 
```bash
awk < file '{ print $2 }'
```

## 分隔符	

默认情况下， awk 使用空格当作分隔符。分割后的字符串可以使用$1, $2等访问。

上面提到过，我们可以使用 -F 来指定分隔符。
fs 如果是一个字符，可以直接跟在-F 后面，比如使用冒号当作分隔符就是 -F: .
如果分隔符比较复杂，就需要使用正则表达式来表示这个分隔符了。
正则表达式需要使用引号引起来。
比如使用‘ab’  当作分隔符，就是 -F 'ab' 了。
使用a或b作为分隔符，就是 -F '\[ab]' 了。
关于正则表达式这里不多说了。


## 内建变量

```text
$0	当前记录（这个变量中存放着整个行的内容）
$1~$n	当前记录的第n个字段，字段间由FS分隔
FS	输入字段分隔符 默认是空格或Tab
NF	当前记录中的字段个数，就是有多少列
NR	已经读出的记录数，就是行号，从1开始，如果有多个文件话，这个值也是不断累加中。
FNR	当前记录数，与NR不同的是，这个值会是各个文件自己的行号
RS	输入的记录分隔符， 默认为换行符
OFS	输出字段分隔符， 默认也是空格
ORS	输出的记录分隔符，默认为换行符
FILENAME	当前输入文件的名字
```

## 模式

```text
~ 表示模式开始
/ /中是模式
!模式取反
```

## 统计

```bash
awk '{sum+=$5} END {print sum}'
```

## 脚本

```text
BEGIN{ 这里面放的是执行前的语句 }
END {这里面放的是处理完所有的行后要执行的语句 }
{这里面放的是处理每一行时要执行的语句}
```

## 运算与编程

awk 是弱类型语言，变量可以是串，也可以是数字，这依赖于实际情况。
所有的数字都是浮点型。

例如

```bash
//9
echo 5 4 | awk '{ print $1 + $2 }'

//54
echo 5 4 | awk '{ print $1 $2 }'

//"5 4"
echo 5 4 | awk '{ print $1, $2 }'

0-1-2-3-4-5-6
echo 6 | awk '{ for (i=0; i<=$0; i++){ printf (i==0?i:"-"i); }printf "\n";}'
```
		
## Example
	
假设我们有一个日期 2014/03/27, 我们想处理为 2014-03-27.
我们可以使用下面的代码实现。

```bash
echo "2014/03/27" | awk -F/  '{print $1"-"$2"-"$3}' 
```

假设 处理的日期都在 date 文件里。
我们可以导入文件来操作

文件名 date

```text
2014/03/27
2014/03/28
2014/03/29
```

命令
```bash
awk -F/ '{printf "%s-%s-%s\n",$1,$2,$3}'  date
```

输出
```text
2014-03-27
2014-03-28
2014-03-29
```


## 转义

一般字符在双引号之内就可以直接原样输出了.  
但是有部分转义字符, 需要使用反斜杠转义才能正常输出.  

```
\\   A literal backslash.
\a   The “alert” character; usually the ASCII BEL character.
\b   backspace.
\f   form-feed.
\n   newline.
\r   carriage return.
\t   horizontal tab.
\v   vertical tab.
\xhex digits
\c   The literal character c.
```

### 单引号

当需要输出单引号时, 直接转义发现会报错.  
由于awk脚本并不是直接执行, 而是会先进行预处理, 所以需要两次转义.  
awk支持递归引号. 单引号内可以输出转义的单引号, 双引号内可以输出转义的双引号.  

比如需要输出单引号, 则需要下面这样:  

```
> awk 'BEGIN{print "\""}'
"
>  awk 'BEGIN{print "'\''"}'
'
```

当然, 更简单的方式是使用十六进制来输出.  

```
awk 'BEGIN{print "\x27"}'
```



## 参考资料

* [gawk的手册 ](http://www.gnu.org/software/gawk/manual/gawk.html)
* [内建变量](http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din-Variables)
* [流控方面](http://www.gnu.org/software/gawk/manual/gawk.html#Statements)
* [内建函数](http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din)
* [正则表达式](http://www.gnu.org/software/gawk/manual/gawk.html#Regexp)


## 推荐

* [awk 基础](awk-base.md)  
* [awk 手册](awk-manual.md)
* [awk 主页](awk.md)

