# Effective C++

## 导读

1. **问题**：何时让析构函数用virtual

2. size_t: c和c++ 中都有这个数据类型，它是sizeof 的返回值，是一种无符号的整型数；

3. 声明：是告诉编译器某个东西的名称和类型(type), 但略去细节；

   定义：提供编译器一些声明中所遗漏的细节。

   ​			对对象而言，编译器为此对象拨发内存的地点；

   ​			对函数或函数模板而言，定义提供代码本体；

   ​			对类或模板类而言，定义列出它们的成员；

4. 初始化：是给对象赋初值的过程。初始化由构造函数执行，所谓的default构造函数，是一个可被调用而不带任何实参的构造函数。**这样的构造函数要不就				是没有参数，要不就是每个参数都有缺省值；**

5. explicit：

   ```shell
   1. explicit: 关键字只能用于修饰只有一个参数的类构造函数，它的作用表示该构造函数是显示的，而非隐式的（显示的代表使用时传入的数据类型必须与定义的一致，隐式的或默认自动做一些数据类型转换）
   2. 类构造函数，默认情况下会声明为implicit(隐式)；
   3. 前面说的“用于修饰只有一个参数的类构造函数",当有多个构造函数，但其他构造函数都有默认值，只有一个参数是没有默认值需要传参的时候，也适用；
   4. google的ｃ++　规范提到explicit的优点是可以避免不合时宜的类型转换，缺点无。所以google约定所有单参数的构造函数都必须是显示的，只有极少数情况下，拷贝构造函数可以不声明为explicit。例如作为其他类的透明包装器的类
   5. effective c++中说：被声明为explicit的构造函数通常比其non-explicit兄弟更受欢迎。因为他们禁止编译器执行非预期（往往也不被期望）的类型转换。除非我有个好理由允许构造函数被用于隐式转换，否则我会把它声明为explicit,鼓励大家遵循相同的政策；
   ```

   

6. 拷贝构造函数：用来以同类型对象初始化自我对象

   ```c++
   class Widget{
       public:
       	Widget();	//default构造函数
       	Widget(const Widget& rhs);	//copy 构造函数：输入参数是自己的类型
   };
   
   Widget w1;	// 调用default构造函数
   Widget w2(w1); //调用拷贝构造函数
   ```

   

7. 拷贝赋值运算符：copy assignment操作符，从另一个同型对象中拷贝其值到自我对象；

   ```c++
   class Widget
   {
       public:
       	Widget();
       	Widget(const Widget& rhs); //copy 构造函数
       	Widget& operator=(const Widget& rhs);//copy assignment 操作符：相当于等号的重载，用等号给同类型对象进行赋值
   }
   
   Widget w1;
   Widget w2(w1);
   w1 = w2; // 调用copy assignment 拷贝赋值运算符:注意这里w1不是一个新的变量
   Widget w3 = w2; //这里调用的是copy构造函数：这里是一个新的对象被定义，一定会有一个构造函数被调用，不能是赋值操作；
   ```

   **注意：**

   - 当出现赋值符号"="是要小心，有时候调用的是拷贝构造函数，有时候调用的是拷贝赋值运算符(copy assignment)，如果有一个新的对象被定义，一定会调用构造函数，此时就不是调用的赋值运算符了，如果是一个“旧的”已经存在的对象，遇到“＝”就是赋值操作，调用的就是拷贝赋值运算符；

   - copy 构造函数很重要，它给了对象passed　by value（以值传递)的可能:

     ```c++
     bool hasAcceptableQuality(Widget w);
     ...
     Widget aWidget;
     if(hasAcceptableQuality(aWidget))...
     // 这里给hasAcceptableQuality函数传参的过程就是pass by value的过程，aWidget被拷贝给w,这里就调用了拷贝构造函数
     // Pass-by-value　意味着调用copy构造函数，以by value传递用户自定义类型是个坏主意，pass-by-reference-to-const是个比较好的选择；
     ```

   - pass-by-reference-to-const 是比较好的选择；

8. 对一个空指针进行取值，会导致“不明确的行为"

   数组越界也会导致"不明确的行为"

   ```c++
   int* p = 0;  //p是一个null 指针
   std::cout << *p;	//对一个null指针取值，导致不明确行为；
   ```

   

## 条款01: 视Ｃ＋＋为一个语言联邦

1. 4个语言联邦：ｃ，Object-oriented C++ (C with Classes),  Template C++, STL; (不会模板和ＳＴＬ相当于只学会了一半的Ｃ++)

2. 函数对象：如果列出一个对象，它的后面跟有由花括号包裹的参数列表，就像ｆ(arg1, arg2, ...)，这个对象就被称为函数对象，最简单的就是函数类，在定义一个类的时候重载了（）运算符，当用该类的对象调用此操作符的时候，其表现形式如同普通函数调用一般，因此取名函数类；其他的还有Lamda, std::function,  std::bind 和闭包

   > https://zhuanlan.zhihu.com/p/87387720

3.  Ｃ＋＋高效变成守则视状况而变化，取决于你用C++的哪一部分。

4. 

## 条款02：尽量以const, enum, inline替换#define

1. #define 会被预处理器处理，编译器不会看到

2. 使用＃define有两个缺点：

   > １．ｄｅｂｕｇ的时候不容易查找问题的原因，因为经过预处理器处理之后，宏定义都被数字所代替了；
   >
   > ２．使用define 比使用const更费内存，因为const一个变量只占一份内存空间，而define需要将所有的宏定义处全部替换掉，会拷贝多份，占的内存空间更大

3. 替代方法一，使用const

   ```
   #define ASPECT_RATIO 1.653    // 宏定义通常使用大写
   
   const double AspectRatio = 1.653
   ```

   <font color='red'>**特殊说明：**</font>

   > 1. 常量指针定义时，有必要将指针声明为const,因为常量通常在头文件中，被不同的人引用，使用const更加安全；
   >
   > 2. 例如在头文件中定义一个常字符串的指针，需要写两次const:
   >
   >    const char* const  authorName = "Scott Meyers";
   >
   >    前一个const修饰char, 证明是一个常字符串；后一个const修饰*，证明是一个常指针；
   >
   >    const 默认修饰左侧的东西，如果左侧没有再修饰右侧的东西；
   >
   >    **上面这个例子：**使用string比char*更适合，const std::string authorName("Scott Meyers");(字符串指针，背后指代或者指向的是一个数组)

4. 替换方法二，仅针对class内的常量，可以使用static const，这样能确保这个常量只有一份，不会占用多的空间

   > １. C++ 要求对所有的东西，除了提供声明还要提供定义；
   >
   > ２．比如在class的头文件中声明
   >
   > ​					class CostEstimate{
   >
   > ​							static const double FudgeFactor;
   >
   > ​					};
   >
   > ​		在一个实现文件中进行定义
   >
   > ​					const double CostEstimate::FudgeFactor = 1.35;  //位于ｃｐｐ，实现文件中					

5. 替换方法三，使用enum

   使用的场景是：当在class内部定义数组时，编译器是一定要知道数组大小的，而如果我们想采用一个常量来表示数组大小，遇到老的编译器，不会让我们直接在static变量上赋初值，会报错，这时我们可以转而使用枚举enum

   > class GamePlayer
   >
   > {
   >
   > ​	private:
   >
   > ​			enmu { NumTruns = 5};  // 令NumTurns成为５的一个记号名称，　the enum hack,作用范围只在class内部
   >
   > ​			int scores[NumTurns];    // 定义数组
   >
   > };

   > １．取一个const的地址是合法的，但取一个enum的地址是不合法的，取一个define的地址也是不合法的。如果你不想让别人获得一个常量的pointer或者reference，可以采用enum;

6. 宏函数

   

   > #define CALL_WITH_MAX(a,b)   f((a) >(b) ?(a):(b))				//注意这种宏函数中，每个参数最好都加上括号
   >
   > int a = 5, b=0;
   >
   > CALL_WITH_MAＸ(++a, b);													//a被累加２次；　把a++ 看成一个整体，调用一次累加一次，最后的结果还会累加一次
   >
   > CALL_WITH_MAX(++a, b+10);												// a被累加１次
   >
   > 
   
   ａ的调用次数取决于ａ和谁进行比较，这样程序就存在着不可预料的行为，安全性降低，解决方案是template inline函数
   
   ```c++
   template<typename T>
   inline void callWithMax(const T& a, const T& b) //这是一个真正的函数，不会在预处理的时候就产生问题　　
   {
       f(a>b ? a:b);
   }
   ```
   
   ==为什么这个可以？＋＋ａ是如何调用的==

==总结==

> 1. 对于单纯的常量，最好以const对象或者enums替换#defines;
>
> 2. 对于形似函数的宏定义，最好改用inline 函数替换掉#define;
>
>    



## 条款03: 尽可能的使用const

1. １．const:默认作用与其左边的东西，如果左边没有东西，则作用于其右边的东西；

2. 迭代器就相当于一个T*，　声明一个迭代器为const就像声明一个指针为const一样，即声明一个Ｔ* * const

3. 常迭代器

   ```
   std::vector<int> vec;
   const std::vector<int>::iterator iter = vec.begin();//const 迭代器　Ｔ*const
   *iter = 10;// 正确；
   ++iter; // 错误；
   
   std::vector<int>::const_iterator cIter = vec.begin(); // const的内容， const T*
   *iter = 10;  //错误，内容不能变；
   ```

   

4. 令函数返回一个常量值，可以降低因客户错误而造成的意外，而又不至于放弃安全性和高效性；

5. 函数的输入参数，除非你有需要改动参数或者local对象，否则请将他们声明为const，他们不过就像local　const对象一样；

6. 改善c++ 程序效率的一个根本办法就是pass by reference-to-const;

   ```
   void print(const TextBlock& ctb)//此函数中ctb是const;
   {
   }
   ```

   

7. class 中两个成员函数，一个返回值有const修饰，一个没有const修饰，其他都一样，这两个函数算重载；

   





