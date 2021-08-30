𝗖𝗦𝟰𝟭𝟴𝟮 𝗖𝗼𝗺𝗽𝘂𝘁𝗲𝗿 𝗚𝗿𝗮𝗽𝗵𝗶𝗰𝘀 𝗖𝗼𝘂𝗿𝘀𝗲 𝗣𝗿𝗼𝗷𝗲𝗰𝘁 - 𝗕𝗮𝘀𝗶𝗰 𝗥𝗲𝗾𝘂𝗶𝗿𝗲𝗺𝗲𝗻𝘁𝘀
𝗡𝗮𝗺𝗲: 𝗣𝗔𝗡𝗚 𝗖𝗵𝗶𝗻𝗴 𝗖𝗵𝗿𝗶𝘀𝘁𝗶𝗲  𝗦𝗜𝗗: 𝟱𝟱𝟳𝟬𝟬𝟴𝟵𝟮
𝗢𝗯𝗷𝗲𝗰𝘁𝗶𝘃𝗲
The objectives of this project is to implement the knowledge on OpenGL/C++ to the real 
application. I aim to practice some basic graphic programming skills here.

𝗕𝗮𝘀𝗶𝗰 𝗥𝗲𝗾𝘂𝗶𝗿𝗲𝗺𝗲𝗻𝘁𝘀 𝗔𝗰𝗵𝗶𝗲𝘃𝗲𝗱
As asked our course leader, Dr.Hou suggested that it is fine that *not* to use the 3D room. 
Therefore, in the project, I implemented a simple sphere and tried to catch-up with 4 of the 
requirements as I want to make something more and make sure the tasks are completed entirelly.
To put it simpler, the requirements implemented in my project include:

1. 𝑪𝒓𝒆𝒂𝒕𝒆 𝑵𝒆𝒘 𝑶𝒃𝒋𝒆𝒄𝒕
   There are two objects in my project, the sphere and the light source respectively. 
   Whereas the sphere is presetted as wireframe, it is colored with the light materials;
   the light source is color as white and can implement 2 specific functions: automatically 
   move and controlled move by ketboard (a,d).

2. 𝑴𝒆𝒏𝒖 𝒂𝒏𝒅 𝑳𝒊𝒈𝒉𝒕𝒊𝒏𝒈
   As required, the pop-up menu in my project is initialised. There are 3 main choices: 
   - Clear:   to clear the sphere into presetted state (wireframe);
   - Light:   there are 6 submenu choices in this section, including ambient, diffuse, specular
              and position; noted there are also MovingLight and KeyboardMovingLight, which is
              not required in this requirements but it's required in other sections.
   - Quit:    To quit the window and end the scheme.

3. 𝑴𝒂𝒏𝒊𝒑𝒖𝒍𝒂𝒕𝒊𝒐𝒏
   It is requested to use keyboard and mouse to manipulate the object(s). 
   In my project, the object can be manipulated (angle) using keyboard. This is also the 
   reason why I used wireframe, which can facilitate the reading of rotating the sphere.
   Hence, when user clicks the KeyboardMovingLight in menu, he/she can use 'a' and 'd' to 
   control and translate the z (position) of the light source object.

4. 𝑨𝒅𝒅𝒊𝒏𝒈 𝑨𝒖𝒕𝒐𝒏𝒐𝒎𝒐𝒖𝒔 𝑶𝒃𝒋𝒆𝒄𝒕𝒔
   Refer to the requirement, set an object to move around automatically and react to the 
   environment (e.g. light)
   Therefore, when user clicks to the MovingLight option, he/she can see the light source 
   (white object) move aroung the sphere and it react to the environment (global embient, 
   specular and spot exponent)

𝗛𝗼𝘄 𝘁𝗼 𝗥𝘂𝗻...
After user press CTRL+F5, the program runs and the window size of 800*800 pop up. 
Then, there is a wireframe of sphere and user can move/rotate angle in pressing the mouse and
move the mouse. Next, user can go to the menu by clicking right mouse. Options will then pop
up and thus, user can choose as they like. For the option "KeyboardMovingLight", user can only
move the light with pressing key 'a' and 'd'. To facilitate the program, you can press them 
long and see the translation.
