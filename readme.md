# **开发文档**

##### **程序功能**：
实现书店管理系统，进行帐户存储与管理、图书信息管理和日志生成与查询。通过命令行交互实现对系统的控制与维护。

##### 主体逻辑：
设计帐户、图书、日志和块状链表四个类，通过类的组合进行相应的信息存储和命令操作。用几个文件来存储图书、帐户和日志的信息。

##### 各个类和文件存储的具体说明：

###### A、帐户类

1.用一个栈来维护登录信息
2.用一个文件存储用户的所有信息（对应一个struct）
3.用另一个文件存用户id与完整信息的位置编号的pair（与块状链表类的组合）
4.设计登录、注销、注册、修改密码、创建帐户、删除帐户等成员函数。

###### B、图书类

1.用一个文件存书籍的全部信息（对应一个struct）
2.用一个文件存书籍ISBN与书籍完整信息结合的pair（与块状链表类结合）
3.再用三个文件分别存储书名、作者、关键字到ISBN的pair
4.设计选择图书、购买、检索、修改信息、进货等成员函数

###### C.日志类

1.用一个文件来存储系统操作类干了什么的信息
2.设计财务查询和日志记录函数
