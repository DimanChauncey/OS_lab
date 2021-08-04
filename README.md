**一、实验项目名称：UESTC OS Lab1进程与资源管理实验**

**二、author：Chauncey_Zhao(zxk)**

**三、时间：2020.5**

**四、实验原理：**

**1.** **总体设计：**

系统总体架构如图1所示，最右边部分为进程与资源管理器，属于操作系统内核的功能。该管理器具有如下功能：完成进程创建、撤销和进程调度；完成多单元资源的管理；完成资源的申请和释放；完成错误检测和定时器中断功能。

![1628049374535](C:\Users\13061\AppData\Roaming\Typora\typora-user-images\1628049374535.png)


图1 系统总体结构

图1中间绿色部分为驱动程序test shell, 设计与实现test shell，该test shell将调度所设计的进程与资源管理器来完成测试。Test shell的应具有的功能：

从终端或者测试文件读取命令；

将用户需求转换成调度内核函数（即调度进程和资源管理器）；

在终端或输出文件中显示结果：如当前运行的进程、错误信息等。

图1最左端部分为：通过终端（如键盘输入）或者测试文件来给出相应的用户命令，以及模拟硬件引起的中断。

**2.Test Shell设计**

Test shell要求完成的命令（Mandatory Commands）：

```shell
-init
-cr <name> <priority>(=1 or 2) // create process 
-de <name> // delete process
-req <resource name> <# of units>  // request resource
-rel <resource name> <# of units>  // release resource
-to  // time out
```

 

**3.** **进程管理设计**

进程状态： ready/running/blocked

进程操作： 

•        创建(create)： (none) -> ready

•        撤销(destroy)： running/ready/blocked -> (none)

•        请求资源(Request): running -> blocked (当资源没有时，进程阻塞)

•        释放资源(Release): blocked -> ready (因申请资源而阻塞的进程被唤醒)

•        时钟中断(Time_out): running -> ready

•        调度：ready -> running  / running ->ready

 

**4.** **进程控制块结构（PCB）**

•        PID（name）

•        resources //: resource which is occupied

•        Status: Type & List// type: ready, block, running…., //List: RL(Ready list) or BL(block list)

•        Creation_tree: Parent/Children

•        Priority: 0, 1, 2 (Init, User, System)

![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image004.png)

图2 PCB结构示意

 

就绪进程队列：Ready list (RL)

![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image006.png)

图3 Ready list 数据结构

 

3个级别的优先级，且优先级固定无变化

2 =“system”

1 = “user”

0 = “init”

每个PCB要么在RL中，要么在block list中 。当前正在运行的进程，根据优先级，可以将其放在RL中相应优先级队列的首部。

 

Init进程在启动时创建，可以用来创建第一个系统进程或者用户进程。新创建的进程或者被唤醒的进程被插入到就绪队列（RL）的末尾。

示例：

图4中，虚线表示进程A为运行进程，在进程A运行过程中，创建用户进程B：cr B 1，数据结构间关系图4所示：

![2](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image008.png)

​                  

图4 进程数据结构间关系

 

**5.** **资源管理设计**

资源的表示：设置固定的资源数量，4类资源，R1，R2，R3，R4，每类资源Ri有i个

资源控制块Resource control block (RCB) 如图5所示

•        RID: 资源的ID

•        Status: 空闲单元的数量

•        Waiting_List: list of blocked process

![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image010.png)

图5 资源数据结构RCB

 

**6.** **进程调度与时钟中断设计**

调度策略：

•        基于3个优先级别的调度：2，1，0

•        使用基于优先级的抢占式调度策略，在同一优先级内使用时间片轮转（RR）

•        基于函数调用来模拟时间共享

•        初始进程(Init process)具有双重作用：

（1）虚设的进程：具有最低的优先级，永远不会被阻塞

（2）进程树的根

 

时钟中断（Time out）：模拟时间片到或者外部硬件中断

 

**7.** **系统初始化设计**

启动时初始化管理器：

具有3个优先级的就绪队列RL初始化；

Init进程；

4类资源，R1，R2，R3，R4，每类资源Ri有i个

 

**五、实验目的：**

设计和实现进程与资源管理，并完成Test shell的编写，以建立系统的进程管理、调度、资源管理和分配的知识体系，从而加深对操作系统进程调度和资源管理功能的宏观理解和微观实现技术的掌握。

 

**六、实验内容：**

在实验室提供的软硬件环境中，设计并实现一个基本的进程与资源管理器。该管理器能够完成进程的控制，如进程创建与撤销、进程的状态转换；能够基于优先级调度算法完成进程的调度，模拟时钟中断，在同优先级进程中采用时间片轮转调度算法进行调度；能够完成资源的分配与释放，并完成进程之间的同步。该管理器同时也能完成从用户终端或者指定文件读取用户命令，通过Test shell模块完成对用户命令的解释，将用户命令转化为对进程与资源控制的具体操作，并将执行结果输出到终端或指定文件中。

 

**七、实验器材（设备、元器件）：**

**Visual Studio 2017****、****windows 10**

**八、实验步骤：**

1.系统功能需求分析：

功能需求在第四部分（实验原理）已经详细描述过，系统总体上由Test Shell、进程管理模块部分、进程控制块结构（PCB）、资源管理模块部分、进程调度与时钟中断模块部分等组成。其中，各部分的原理以及具体结构在总体框架设计和具体实现部分描述，此处不再赘述，具体分析与代码注释详见后面部分。

 

2.总体框架设计：

（a）项目的总体框架与结构如图所示：

![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg)

其中shell.cpp包含系统的主函数部分，主要对应实现的是Test Shell部分，根据输入的命令进行解析，调用系统的不同功能模块，并输出相应结果。

（b）各模块的介绍及数据结构：

PCB.h与PCB.cpp中定义了PCB类以及其成员函数。PCB类是整个系统的主要核心数据结构，定义了PCB的成员变量，便于操作系统对进程的管理。其中，资源占有变量resource_occupied的数据结构采用了map，c++中STL的map模板类底层采用了红黑树，查找效率极快。子进程列表Children采用了链表list的数据结构。

Resource.h定义了resource类以及其成员函数。该类定义了资源的数据结构，实现了对应的get方法，便于操作系统实现请求资源、释放资源、输出资源信息等功能。其中资源的等待队列使用了list<pair<PCB*,int>>的数据结构，因为list有方便的自带操作，且使用pair的数据结构把进程和进程需要申请的资源数一并记录下来。

os.h与os.cpp对操作系统的内核功能进行了实现，其中定义了系统中几个重要的全局变量，定义了操作系统的内核函数，主要实现了进程的创建、进程的切换与调度、进程信息的输出、资源请求、释放资源等等功能，函数功能如前面部分所述。

（c）模块引用关系：

![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image014.png)

（d）总体工作流程：

用户输入命令后，在shell.cpp（内有主函数）中解析命令，并根据解析后的命令来在os.h（内核函数模块）中调用相应的内核函数运行，最终由shell.cpp输出运行结果

（e）各模块原理：

详细设计部分的代码中有对原理的详细注释，在此不进行赘述。

 

3.详细设计：
见代码


**九、实验数据及结果分析：**

实验数据及结果分析要求:

  给出测试流程和测试结果（针对实验1）

  给出实验结果分析

输入测试命令或将测试命令放在测试文件input.txt中，内容为：
```shell
1.  cr x 1   
2.  cr p 1  
3.  cr q 1  
4.  cr r 1  
5.  to  
6.  req R2 1  
7.  to  
8.  req R3 3  
9.  to  
10. req R4 3  
11. to  
12. to  
13. req R3 1  
14. req R4 2  
15. req R2 2  
16. to  
17. de q  
18. to  
19. to  
```


   **结果分析：**

首先，在系统初始启动时，创建init进程，其优先级为0，输出当前正在执行的进程为init。初始化资源R1、R2、R3、R4，数量分别为1、2、3、4。

随后，输入命令“cr x 1”创建进程x，其优先级为1，由于x的优先级高于init进程，因此，创建完成后即调度进程x执行，输出当前正在执行的进程为x。

接下来，输入命令“cr p 1”“cr q 1”“cr r 1”依次创建进程p、q、r，由于这3个进程的优先级都是1，不高于当前执行的进程x，因此，创建完成后系统当前执行进程依然为x，分别输出当前执行进程为x，此时优先级为1的就绪队列中依次为进程p、q、r。

输入命令“to”后，进程x时间片完，回到就绪队列。系统调度执行进程p，输出当前执行进程为p，此时优先级为1的就绪队列中依次为进程q、r、x。

输入命令“req R2 1”，为进程p申请1个R2资源。由于申请数量少于R2资源剩余数量，因此可以申请，不会发生阻塞，输出当前执行进程为p，R2资源剩余量为2-1=1，优先级为1的就绪队列中依次为进程q、r、x。

输入命令“to”后，进程p时间片完，回到就绪队列。系统调度执行进程q，输出当前执行进程为q，此时优先级为1的就绪队列中依次为进程r、x、p。

输入命令“req R3 3”，为进程q申请3个R3资源。由于申请数量少于R3资源剩余数量，因此可以申请，不会发生阻塞，输出当前执行进程为q，R3资源剩余量为3-3=0，优先级为1的就绪队列中依次为进程r、x、p。

输入命令“to”后，进程q时间片完，回到就绪队列。系统调度执行进程r，输出当前执行进程为r，此时优先级为1的就绪队列中依次为进程x、p、q。

输入命令“req R4 3”，为进程r申请3个R4资源。由于申请数量少于R4资源剩余数量，因此可以申请，不会发生阻塞，输出当前执行进程为r，R4资源剩余量为4-3=1，优先级为1的就绪队列中依次为进程x、p、q。

输入命令“to”后，进程r时间片完，回到就绪队列。系统调度执行进程x，输出当前执行进程为x，此时优先级为1的就绪队列中依次为进程p、q、r。

输入命令“to”后，进程x时间片完，回到就绪队列。系统调度执行进程p，输出当前执行进程为p，此时优先级为1的就绪队列中依次为进程q、r、x。

输入命令“req R3 1”，为进程p申请1个R3资源。由于R3资源剩余数量为0，小于申请数量，因此失败，进程p阻塞，系统调度执行进程q，输出当前执行进程为q，R3资源剩余量为0，优先级为1的就绪队列中依次为进程r、x，R3资源的阻塞队列为p。

输入命令“req R4 2”，为进程q申请2个R4资源。由于R4资源剩余数量为1，小于申请数量，因此失败，进程q阻塞，系统调度执行进程r，输出当前执行进程为r，R4资源剩余量为1，优先级为1的就绪队列中依次为进程x，R4资源的阻塞队列为q。

输入命令“req R2 2”，为进程r申请2个R2资源。由于R2资源剩余数量为1，小于申请数量，因此失败，进程r阻塞，系统调度执行进程x，输出当前执行进程为x，R2资源剩余量为1，此时优先级为1的就绪队列为空，R2资源的阻塞队列为r。

输入命令“to”后，进程x时间片完，回到就绪队列，但此时优先级为1的就绪队列为空，因此x是该就绪队列唯一的进程，系统依然调度执行进程x，输出当前执行进程为x，此时优先级为1的就绪队列依然为空。

输入命令“de q”后，删除进程q以及其子进程，由于进程q没有子进程，因此只删除了进程q自身，并释放其持有的3个R3资源。资源释放后，资源R3阻塞队列的进程p可以成功申请到其需要申请的1个R3资源，因此p回到优先级为1的就绪队列中。R3资源剩余量为3-1=2，此时优先级为1的就绪队列只有进程p。但由于进程p的优先级与当前正在执行的进程x优先级相同，不能抢占，因此系统依然执行进程x，输出当前执行进程为x。

输入命令“to”后，进程x时间片完，回到就绪队列。系统调度执行进程p，输出当前执行进程为p，此时优先级为1的就绪队列中只有进程x。

输入命令“to”后，进程p时间片完，回到就绪队列。系统调度执行进程x，输出当前执行进程为x，此时优先级为1的就绪队列中只有进程p。

​    综上所述，输出结果为：

![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)![img](file:///C:/Users/13061/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)

与实验指导书中给出的预期输出结果一致。

 

**十、实验结论：**

通过本次实验，进一步巩固了课堂所学的进程管理和资源管理的相关知识，并在熟练掌握课堂所学内容的基础上，使用C++语言简单模拟了计算机操作系统对进程和资源的管理和调度，系统能顺利通过测试用例，取得与预期一致的结果，说明实现取得成功，加深了对操作系统进程调度和资源管理功能的理解。