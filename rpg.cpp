#include "gridclass.hpp"
#include <conio.h>
#include <vector>

class npc:public obj
{
    bool talking;
    vector<bundle> text = vector<bundle>(); //issue with
    //bundle testtext;
    int curtext;

    public:
        npc(string name, int x, int y, char sym, enum Colour colour) : obj(name, x, y, sym, colour)
        {
            talking = false;
            curtext = 0;
            //testtext = bundle();
        }
        npc() : obj()
        {
            talking = false;
            curtext = 0;
            //testtext = bundle();
        }

        void addtext(string filename, enum Colour colour)
        {
            const enum Colour specialcolour = YELLOW;
            static int offset = 0;
            char buffer[100];
            string tempname = name + itoa(offset, buffer, 10);
            bundle temp = bundle(tempname, filename);

            temp.setallcolour(colour);
            bool special = false;
            int x = 0;
            int cury = 0;
            for (int i = 0; i < temp.getparts().size(); i++)
            {
                if (temp.gety(i) != cury)
                {
                    x = 0;
                }
                cury = temp.gety(i);
                if (temp.getpart(i).getsym() == '<')
                {
                    //temp.setallcolour(specialcolour); //test works
                    special = true;
                    x += 1;
                    continue;
                }
                else if (temp.getpart(i).getsym() == '>')
                {
                    special = false;
                    x += 1;
                    continue;
                }
                // else if (temp.getpart(i).getsym() == '\n') //not executing, probably because '\n' is not being saved. instead detect changes in the y value
                // {
                //     temp.setallcolour(specialcolour); //test works
                //     x = 0;
                //     continue;
                // }
                
                if (special)
                {
                    temp.setcolour(i, specialcolour);
                }
                temp.setx(i, temp.getpart(i).getx() - x);

            }
            for (int i = 0; i < temp.getparts().size(); i++)
            {
                if (temp.getpart(i).getsym() == '<' || temp.getpart(i).getsym() == '>')
                {
                    //temp.setallcolour(specialcolour); test works
                    //temp.setsym(i, '\0');
                    temp.removepart(i);
                }

            }

            
            text.push_back(temp);
            //testtext = bundle(tempname, filename);
            offset += 1;
        }

        void talk()
        {
            //cout << " is talking";
            talking = true;
        }
        void stoptalk()
        {
            talking = false;
            //printf("I have stopped talking!\n");
            curtext = 0;
        }

        bool istalking()
        {
            return talking;
        }
        bool donetalking()
        {
            //return false;
            return curtext % text.size() == 0;
        }

        bundle gettext(int start = 0)
        {
            //printf("\ndo you run");
            int offset = start + curtext;
            //printf("\ndo you run");
            //return testtext;
            return text[start]; //issue
        }
        bool detect()
        {
            //printf("detect me!");
            bool detected = false;
            if (kbhit())
            {
                //printf("detect me!");
                char key = getch();
                if (key == ' ')
                {
                    //printf("\nmega based");
                    curtext += 1;
                    detected = true; //now works
                }
            }
            //printf("detect me!");
            return detected;
        }

};

class sgrid: public grid
{
    vector<npc> npclist;
    public:
        sgrid(int length, int height, char background = ' '): grid(length, height, background)
        {
            npclist = vector<npc>();
        }

        void addnpc(npc NPC)
        {
            npclist.push_back(NPC);
        }
        npc getnpc(int index)
        {
            return npclist[index];
        }
        int getnumnpc()
        {
            return npclist.size();
        }
        void makenpctalk(int index)
        {
            npclist[index].talk();
        }
        void removenpc(npc NPC)
        {
            for (int i = 0; i < npclist.size(); i++)
            {
                if (NPC.getname() == npclist[i].getname())
                {
                    npclist.erase(npclist.begin() + i);
                    break;
                }
            }
        }
        void updatenpc(npc NPC)
        {
            for (int i = 0; i < npclist.size(); i++)
            {
                if (NPC.getname() == npclist[i].getname())
                {
                    npclist[i] = NPC;
                    break;
                }
            }
        }
        
};

//finish npc in isvalid
class player: public obj
{
    npc target;
    bool nearnpc;
    bool isvalid(char key, sgrid *display)
    {
        bool valid = true;
        //for now do walls and npcs different
        switch (key)
        {
            case 'r':
                if (x == (*display).getlength() - 1)
                {
                    valid = false;
                }
                break;
            case 'l':
                if (x == 0)
                {
                    valid = false;
                }
                break;
            case 'u':
                if (y == 0)
                {
                    valid = false;
                }
                break;
            case 'd':
                if (y == (*display).getheight() - 1)
                {
                    valid = false;
                }
                break;
        }
        for (int i = 0; i < (*display).getnumnpc(); i++)
        {
            switch (key)
            {
                case 'r':
                    if (x == (*display).getnpc(i).getx() - 1 && y == (*display).getnpc(i).gety())
                    {
                        valid = false;
                    }
                    break;
                case 'l':
                    if (x == (*display).getnpc(i).getx() + 1 && y == (*display).getnpc(i).gety())
                    {
                        valid = false;
                    }
                    break;
                case 'u':
                    if (y == (*display).getnpc(i).gety() + 1 && x == (*display).getnpc(i).getx())
                    {
                        valid = false;
                    }
                    break;
                case 'd':
                    if (y == (*display).getnpc(i).gety() - 1 && x == (*display).getnpc(i).getx())
                    {
                        valid = false;
                    }
                    break;
            }
        }


        return valid;

    }
    //it works
    bool isnearnpc(sgrid *display)
    {
        nearnpc = false;
        for (int i = 0; i < (*display).getnumnpc(); i++)
        {
            if ((x == (*display).getnpc(i).getx() - 1 && y == (*display).getnpc(i).gety()) ||
            (x == (*display).getnpc(i).getx() + 1 && y == (*display).getnpc(i).gety()) ||
            (y == (*display).getnpc(i).gety() + 1 && x == (*display).getnpc(i).getx()) ||
            (y == (*display).getnpc(i).gety() - 1 && x == (*display).getnpc(i).getx()))
            {
                //printf("%s", display.getnpc(i).getname());
                //cout << display.getnpc(i).getname();
                //(*display).getnpc(i).talk();
                //target = (*display).getnpc(i);
                (*display).makenpctalk(i);
                //cout << target.getname();
                nearnpc = true;
                break;
            }
        }
        return nearnpc;
    }
    void right(sgrid *display)
    {
        if (isvalid('r', display))
        {
            x += 1;
        }
    }
    void left(sgrid *display)
    {
        if (isvalid('l', display))
        {
            x -= 1;
        }
    }
    void up(sgrid *display)
    {
        if (isvalid('u', display))
        {
            y -= 1;
        }
    }
    void down(sgrid *display)
    {
        if (isvalid('d', display))
        {
            y += 1;
        }
    }
    void npcinteract(sgrid *display)
    {
        if (isnearnpc(display))
        {
            //printf("yo");
            //target.talk();
            //display.updatenpc(target);

        }
    }

    public:
        player(string name, int x, int y, char sym, enum Colour colour) : obj(name, x, y, sym, colour)
        {
            target = npc();
            nearnpc = false;
        }

        bool detect(sgrid *display)
        {
            bool detected = false;
            if (kbhit())
            {
                char key = getch();
                if (key == 'w')
                {
                    up(display);
                    detected = true;
                }
                else if (key == 'a')
                {
                    left(display);
                    detected = true;
                }
                else if (key == 's')
                {
                    down(display);
                    detected = true;
                }
                else if (key == 'd')
                {
                    right(display);
                    detected = true;
                }
                else if (key == ' ')
                {
                    //printf("HELLO");
                    npcinteract(display);
                    detected = true;
                }
            }
            return detected;
        }
        // bool isnearnpc(sgrid display)
        // {
        //     nearnpc = false;
        //     for (int i = 0; i < display.getnumnpc(); i++)
        //     {
        //         if ((x == display.getnpc(i).getx() - 1 && y == display.getnpc(i).gety()) ||
        //         (x == display.getnpc(i).getx() + 1 && y == display.getnpc(i).gety()) ||
        //         (y == display.getnpc(i).gety() + 1 && x == display.getnpc(i).getx()) ||
        //         (y == display.getnpc(i).gety() - 1 && x == display.getnpc(i).getx()))
        //         {
        //             target = display.getnpc(i);
        //             nearnpc = true;
        //             break;
        //         }
        //     }
        //     return nearnpc;
        // }

};

//maybe make it so different part of npc text are different colours (ie highlighting special text)
int main(void)
{
    sgrid display = sgrid(30, 20);
    player p = player("player", 5, 5, '@', RED);
    //p.setcolour(RED);

    npc fred = npc("fred", 10, 10, 'F', BLUE);
    //fred.setcolour(RED);
    fred.addtext("text1.txt", RED);
    fred.addtext("text2.txt", RED);

    npc max = npc("max", 15, 10, 'M', YELLOW);
    //max.setcolour(GREEN);
    max.addtext("maxtext1.txt", GREEN);
    max.addtext("maxtext2.txt", BLUE);
    max.addtext("maxtext3.txt", RED);
    

    int curtext = 0;
    npc target = npc();
    bool npctalking = false;
    bool npcsetup = false;
    bundle lasttext = bundle();
    display.addobj(p);
    display.addobj(fred);
    display.addobj(max);
    display.addnpc(fred);
    display.addnpc(max);

    bool going = true;
    while (going)
    {
        if (npctalking)
        {
            //printf("\nsigma");
            //lasttext = target.gettext();
            //printf("\nsigma");
            //display.addbundle(target.gettext()); //stops here
            //printf("\nalpha");
            //lasttext = target.gettext();
            //printf("\nbeta");
            if (!npcsetup)
            {
                npcsetup = true;
                display.addbundle(target.gettext());
                lasttext = target.gettext();
            }
            if (target.detect())
            //if (kbhit())
            {
                // char key = getch();
                // if (key == ' ')
                // {
                //     curtext += 1;
                //     printf("\ndelta");
                //     // if (!target.donetalking())
                //     if (curtext != 2)
                //     {
                //         //printf("WAAHAHAAHHAHAHAAHAHAHAHAAAHAHAHAHAHAHAHAHAHAHAHAHAHAHAAHAHAHAH");//now works
                //         display.removebundle(lasttext);
                //         display.addbundle(target.gettext(curtext));
                //         lasttext = target.gettext(curtext);
                //     }
                //     else
                //     {
                //         npctalking = false;
                //     }
                // }
                //printf("\ndelta");
                if (!target.donetalking())
                //if (curtext != 1)
                {
                    curtext += 1;
                    //printf("WAAHAHAAHHAHAHAAHAHAHAHAAAHAHAHAHAHAHAHAHAHAHAHAHAHAHAAHAHAHAH");//now works
                    display.removebundle(lasttext);
                    display.addbundle(target.gettext(curtext));
                    lasttext = target.gettext(curtext);
                }
                else
                {
                    npctalking = false;
                    target.stoptalk(); //this was the solution, moving around code
                    display.updatenpc(target); 
                    //printf("Mario and luigi"); //this is happening
                }
                
            }
            // if (target.donetalking())
            // {
            //     npctalking = false;
            //     curtext = 0;
            //     display.removebundle(lasttext);
            //     npcsetup = false;
            // }
        }
        else
        {
            //npctalking = false;
            curtext = 0;
            display.removebundle(lasttext);
            npcsetup = false;
            // target.stoptalk(); //this is not happening
            // display.updatenpc(target); //or this
            //display.getnpc(0).stoptalk();
            //display.updatenpc(display.getnpc(0));
            //printf("Whats up beijing");
        }
        // if (display.getnpc(0).istalking())
        // {
        //     printf("Freddddddddddddd");
        // }
        if (p.detect(&display) && !npctalking)
        {
            //printf("hello");
            display.updateobj(p);
        }
        for (int i = 0; i < display.getnumnpc(); i++)
        {

            // if (display.getnpc(i).istalking())
            // {
            // }
            if (display.getnpc(i).istalking() && !npctalking)
            {
                //cout << "it works\n";
                target = display.getnpc(i);
                //cout << target.getname() << "\n";
                npctalking = true;
            }
        }
        // if (npctalking)
        // {
        //     //printf("\nsigma");
        //     //lasttext = target.gettext();
        //     //printf("\nsigma");
        //     //display.addbundle(target.gettext()); //stops here
        //     //printf("\nalpha");
        //     //lasttext = target.gettext();
        //     //printf("\nbeta");
        //     if (!npcsetup)
        //     {
        //         npcsetup = true;
        //         display.addbundle(target.gettext());
        //         lasttext = target.gettext();
        //     }
        //     // if (target.detect())
        //     if (kbhit())
        //     {
        //         char key = getch();
        //         if (key == ' ')
        //         {
        //             curtext += 1;
        //             printf("\ndelta");
        //             // if (!target.donetalking())
        //             if (curtext != 3)
        //             {
        //                 printf("WAAHAHAAHHAHAHAAHAHAHAHAAAHAHAHAHAHAHAHAHAHAHAHAHAHAHAAHAHAHAH");//now works
        //                 display.removebundle(lasttext);
        //                 display.addbundle(target.gettext(curtext));
        //                 lasttext = target.gettext(curtext);
        //             }
        //             else
        //             {
        //                 npctalking = false;
        //             }
        //         }
                
        //     }
        //     // if (target.donetalking())
        //     // {
        //     //     npctalking = false;
        //     //     curtext = 0;
        //     //     display.removebundle(lasttext);
        //     //     npcsetup = false;
        //     // }
        // }
        // else
        // {
        //     // npctalking = false;
        //     curtext = 0;
        //     display.removebundle(lasttext);
        //     npcsetup = false;
        //     target.stoptalk();
        //     display.updatenpc(target);
        //     printf("Whats up beijing");
        // }
        // if (curtext < 3 && npctalking)
        // {
        //     wait(500);
        // }
        //cout << curtext;
        display.display();
    }
    return 0;
}