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

8. const 函数，只能修改指针变量，不能修改non-static 成员变量，如果const成员函数想修改non-static成员变量，可以将该变量做成mutable(可变的)，释放掉non-static成员变量的约束；

   ```
   class cTextBlock
   {
   	public:
   		char* pText;
   		mutable std::size_t textLength;
   		mutable bool lengthIsValid;		//这些成员变量可能总是会被更改，即使在const成员函数中；
   		std::size_t length() const;
   };
   
   std::size_t length() const
   {
   	if(!lengthIsValid)
   	{
   		textLength = std:strlen(pText);// const 函数修改了non-static的成员变量；
   	}
   	return textLength;
   }
   ```

   > １．const 函数不能改变成员数据（static 除外）, 只能读取数据成员，不能改变数据成员;
   >
   > ２．const 对象只能调用const成员函数，不能调用非const的成员函数；
   >
   > 

9. ==边界检验　志记访问==

10. ==使用non-const 函数调用它自己的const函数，是一个避免代码重复的安全做法；==前提是当const 和non-const 成员函数有着实质等价的实现；

## 条款0４: 确定对象被使用前已经被初始化

1. 对于自定义类型，确保每一个构造函数都将对象的每一个成员初始化；

2. 使用构造函数初始化的时候，采用初始化列表，而不要直接在进入构造函数之后再初始化。如果在构造函数之后再初始化，会先调用default构造函数，然后再调用copy assignment函数进行赋值，效率低。

3. "成员初始化的次序"：base classes 早于derived classes, class 的成员变量以其声明次序进行初始化；

4. ==将每个non-local static 对象搬到自己的专属函数内，并且将该对象在此函数内声明为static，函数返回一个reference指向该对象，然后用户调用这些函数，而不是直接指向这些对象==: 

   > １．表现是：在一个class内部定义了一个自己同名的class对象，且是静态的；
   >
   > ２．用户可以从函数得到对象；

   ==任何一种non-const static对象，无论它是local或者no-local，在多线程环境下“等待某事发生"都会有麻烦==

5. 为内置对象进行手工初始化，因为c++不保证初始化他们；

   

   

## 条款0５: 了解Ｃ＋＋默默编写并调用哪些函数

1. 如果你在一个ｃｌａｓｓ中什么都没写，那么编译器或为它声明一个default构造函数，一个copy构造函数，一个copy assignment操作符和一个析构函数，这些函数都是==public 且inline==

   > 当然，只有当你需要这些函数的时候，编译器才会去创建它

2. ==编译器产生的析构函数是个non-virtual==

3. ==编译器为你写函数，这些函数做了什么？==

   > 1. default 构造函数和析构函数，主要是给编译器一个地方用来放置”藏身幕后"的代码，像是调用base classes和no-static成员变量的构造函数和析构函数；
   > 2. copy 构造函数和copy assignment操作符，编译器创建的版本只是单纯地将来源对象的每一个non-static 成员变量拷贝到目标对象；

4. c++ 不允许“让reference　改指向不同对象"

5. ==当class里面有const成员变量和reference成员的时候，编译器自动生成的 copy构造函数不能执行拷贝操作，会报错;==

   


## 条款06: 若不想使用编译器自动生成的函数，就该明确拒绝；

> 1. 编译器自动生成的函数都是public;
> 2. 如果你想阻止编译器自动生成某个函数，那么在private里面定义一个，就能阻止外面的人调用默认生成的函数（不是最优解：因为成员函数和友元函数还是能调用）；(如果误用，编译器抱怨)
> 3. 最优的解法是：声明在private中，并且不去实现；（如果误用，连接器抱怨）
> 4. 对于只声明，不实现的函数，其输入的形参也可以不写；
> 5. 更妙的写法是定义一个不能拷贝的基类：在基类中定义private的拷贝构造函数和赋值构造函数，然后再继承这个函数，继承者的对象也不能实施拷贝和赋值构造的操作；

## 条款0７: 为多态基类声明virtual析构函数

==工厂函数，工厂模式==

> 1. 　析构函数的目的是允许派生类的（derived class）实现得以==客制化==
>
> 2. 　当基类的析构函数没有申明虚析构函数时，派生类从基类继承，基类的指针指向派生类对象时（使用虚函数的情况时），delete
>
>     基类的指针时，只会调用基类的析构函数，不会调用派生类的析构函数,　容易造成内存泄露；解决的办法是加一个虚析构函数；
>
> 3. 　当一个class里面有virtual函数时，其体积会增大，需要额外的信息指出虚构函数，通常采用的是vpr,是有函数指针构成的数组
>
> 4. 　当基类中定义了虚函数时，基类的析构函数需要使用虚析构函数，以防止内存泄露：当class内至少有一个virtual函数时，应该声明一个virtual析构函数；
>
> 5. 　==纯虚函数导致abstract(抽象)　class，也就是不能被实体化的class==
>
> 6. class 的设计目的如果不是作为base class 使用，或者不是为了具备多态性，就不该声明为virtual析构函数；



## 条款0８: 别让异常逃离析构函数

> １．一个类如果在析构函数中抛出了异常，很容易导致不确定的行为，或者导致内存泄露；
>
> ２．如果非要在析构函数中抛出异常，可以使用std::abort()，抢先置“不明确行为"于死地；
>
> ３．==更好的方法是重新设计接口，使客户有机会对可能出现的问题做出反应==
>
> ４. 如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么class应该提供一个而普通函数（而非析构函数中)执行该操作；

## 条款09: 绝不在构造和析构过程中调用virtual函数

> １．基类的构造函数比派生类的构造函数先调用；
>
> ２．不要在构造函数或者析构函数里面调用虚函数；
>
> ３．解决方案，可以在基类中定义non-virtual函数，要求派生类给它传值实现构造过程；
>
> ４．初始化成员列表可以改用一个static 函数实现，这样更加简明可读性强；

## 条款10: 令operator= 返回一个reference to *this

> 1. 　在重构赋值操作符的时候，令赋值操作符返回一个　reference to *this：　就是函数的返回值定义为reference类型，函数结尾return的时候为 *this
> 2.    这里的赋值操作符包括：=, +=, -=, *=, /= 等；

## 条款11: 在operator= 中处理“自我赋值"

> 1. 不明显的自我赋值是”别名“带来的后果，所谓的别名，是指有一个以上的方法指向同一个对象；
>
> 2. 为了避免”自我赋值"时误删一些东西，在自我赋值前先进行“证同测试”，　然后不做任何事情，直接把值赋过去；
>
>    if(this = &rhs) return *this;
>
> 3. 也可以先对自我进行备份，记住它，然后再创建新的，最后再删除；（需要精心安排顺序)
>
> 4. 还有一个方案，使用copy and swap 技术，交换*this和rhs的内容，其本质是，默认生成的拷贝构造函数“以by value方式接受实参数" , 以by value方式传递东西会造成一份复件(副本)
>
> 5. 确定任何函数如果操作一个以上的对象，而其中多个对象是同一个对象时，其行为仍然正确；
>
>    

## 条款03: 尽可能的使用const

## 条款03: 尽可能的使用const

## 条款03: 尽可能的使用const











