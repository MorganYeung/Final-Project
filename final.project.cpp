/*
This code is the one that was ran by the NXt that was attached to the car. Its main purpose is to drive the car and the arm attached.
It also communicated with the slave NXT through bluetooth and told it when to run the conveyer belts.
*/
 
#include "NXTServo-lib-UW.c" //Needed to control servo motors
 
struct Sandwich
/* The stuct "Sandwich" will serve as storage for
the toppings on the Sandwich. There will be a boolean
for each topping choice. True means that that topping
will be put on the Sandwich and false means it will not.
*/
{
        bool cheese;
        bool meat;
        bool lettuce;
        bool ketchup;
};
 
struct Sandwich sandwich1;
/*
declares a sandwich struct as a global variable
so that it can be called in the functions
*/
 
void customize() // Written by Eric Tweedle
/* 
This function walks the user through the steps of desinging
their Sandwich. It will use the NXT buttons and screen to interface
with the user. It is called by the function preselect.
*/
{
        eraseDisplay();
        wait1Msec(1000);
        int stage = 1;
        string SandwichItems[4]; // will keep track of selected toppings
//so they can be displayed later
        displayString(0, "would you like");
        displayString(1, "cheese?");
        displayString(5,"YES                                                                    NO");
while (stage == 1) // while loops keep the screen up until the
//user makes a choice
        {
                if (nNxtButtonPressed == 1) // The button on the right //representing no is pressed
                {
                        sandwich1.cheese = false;
                        eraseDisplay();
                        stage++; //moves to the next choice
                }
                if (nNxtButtonPressed == 2) // The button on the left
//representing yes is pressed
                {
                        sandwich1.cheese = true;
                        eraseDisplay();
                        SandwichItems[0] = "Cheese";
                        stage ++;
                }
        }
        wait1Msec(1000); // Wait to prevent skipping of choices
        displayString(0, "would you like");
        displayString(1, "meat?");
        displayString(5,"YES                                                                    NO");
 
        while (stage == 2)
        {
                if (nNxtButtonPressed == 1)
                {
                        sandwich1.meat = false;
                        eraseDisplay();
                        stage ++;
                }
                if (nNxtButtonPressed == 2)
                {
                        sandwich1.meat = true;
                        eraseDisplay();
                        SandwichItems[1] = "Meat";
                        stage ++;
                }
        }
        wait1Msec(1000);
        displayString(0, "would you like");
        displayString(1, "lettuce?");
        displayString(5,"YES                                                                    NO");
 
        while (stage == 3)
        {
                if (nNxtButtonPressed == 1)
                {
                        sandwich1.lettuce = false;
                        eraseDisplay();
                        stage ++;
                }
                if (nNxtButtonPressed == 2)
                {
                        sandwich1.lettuce = true;
                        eraseDisplay();
                        SandwichItems[2] = "Lettuce";
                        stage ++;
                }
        }
        wait1Msec(1000);
        displayString(0, "would you like");
        displayString(1, "Ketchup?");
        displayString(5,"YES                                                                    NO");
 
        while (stage == 4)
        {
                if (nNxtButtonPressed == 1)
                {
                        sandwich1.ketchup = false;
                        eraseDisplay();
                        stage ++;
                }
                if (nNxtButtonPressed == 2)
                {
                        sandwich1.ketchup = true;
                        eraseDisplay();
                        SandwichItems[3] = "Ketchup";
                        stage ++;
                }
        }
        /* the following 6 lines will display to the screen what
 is on the costumized Sandwich. The display is set just as the sandwich will be made
*/
        displayString(0,"Your sandwich:");
        displayString(1,"bread");
 
        for (int i = 3; i >= 0; i--)
        {
                displayString((i+2), SandwichItems[i]);
        }
        displayString(6, "bread");
 
        wait1Msec(3000);
}
 
 
int preselect() // written by Ben Holmes
/* preselect will interface with the user using the NXT buttons and screen.
The user will be asked to choose between a preselct Sandwich type or to choose
their own. If they want a preselect Sandwich they will be given the choice between
two premade Sandwich types. If they do not then the customize function will be ran.
The int being returned represents what the user has chosen and changes the struct
accordingly in main.
*/
{
        bool isPreselect = false; // represents whether the user chooses a
//preselect sandwich.
        do
        {
        displayString(0, "Customizable");
        displayString(1, "Sandwich?");
        displayString(5,"YES                                                                    NO");
        if (nNxtButtonPressed == 2) // If the left button is pressed then
//run the customize function
        {
                customize();
                eraseDisplay();
                return 0;
        }
        if (nNxtButtonPressed == 1)// if the right buttin is pressed
//change the isPreselect bool to true.
        {
                isPreselect = true;
                eraseDisplay();
        }
        }while (nNxtButtonPressed != 2 && nNxtButtonPressed != 1); // does
//the above while waiting for a button press
        while (isPreselect)//runs if the user chose a preselect sandwich
//type.
        {
                wait1Msec(500);
                displayString(0, "Choose a");
                displayString(1, "sandwich type");
                displayString(4, "cheese                veggy");
                displayString(6, "    Orange for");
                displayString(7, "    customize");
                if (nNxtButtonPressed == 2) // cheese sandwich chosen and
//related int returned
                {
                        eraseDisplay();
                        displayString(0, " You chose a");
                        displayString(1, "cheese sandwich");
                        wait1Msec(2000);
                        eraseDisplay();
                        return 1;
                }
                if (nNxtButtonPressed == 1) // vegan sandwich chosen and
//related int returned
                {
                        eraseDisplay();
                        displayString(0, " You chose a");
                        displayString(1, "vegan sandwich");
                        wait1Msec(2000);
                        eraseDisplay();
                        return 2;
                }
                if(nNxtButtonPressed == 3) // user changed their mind.
//customize function is ran.
                {
                customize();
                eraseDisplay();
                return 0;
                }
        }
        return 0;
}
 
void getItem (string item) // Written by Morgan Yeung
/*
The getItem function takes in a string that represents what item it is getting. Depending on the string it will send a message
to the Slave NXT through bluetooth that indicates the conveyer with the desired item. For cheese it swings the servo arm to hit a touch
sesor which is connected to another NXT that runs the cheese conveyer belt.
*/
{
        int gripperArm = 1; // servo initialization.
        int gripper = 3;                // servo initialization.
        int car = 2;                            // servo initialization.
        bool NotTwoRed = true; //used since there are two red colour strips. True means to stop at the first strip.
        bool NotTwoBlue = true; //used since there are two blue colour strips. True means to stop at the first strip.
        int my_message = 0; //The integer that is sent to the Slave NXT through bluetooth
   if(item=="cheese")// runs if cheese is requested
    {
      setServoSpeed(S1, car, 20); // moves the car forward
                while (SensorValue[S2] != REDCOLOR) // waits for the colour sensor to see a red strip.
                {}
        setServoSpeed(S1, car, 0); // stops the car
        //below: swings the arm to hit a touch sensor.
                setGripperPosition(S1, gripper, 0);
                        wait1Msec(200);
                        setServoPosition(S1, gripperArm, -20);
                        wait1Msec(500);
                        setServoPosition(S1, gripperArm, 70);
                        wait1Msec(3000);
                        NotTwoRed = false; // indicates that it has stopped at one red strip already.
        }
 
    if(item=="meat") // runs if the meat is requested on the sandwich.
        {
        setServoSpeed(S1, car, 20);
        while(SensorValue[S2] != REDCOLOR) // waits for the colour sensor
//to detect red.
        {}
                if(NotTwoRed) // runs if cheese was not requested
                {
                        wait1Msec(1000);
                        while(SensorValue[S2] != REDCOLOR) // waits for a 
//second red strip
                                {}
                }
        setServoSpeed(S1, car, 0);
        my_message = 3; // initialized a message to the Slave to run the
//meat conveyer.
                sendMessage(my_message); // sends the message over
//bluetooth
                wait1Msec(3000);
        }
 
    if(item=="lettuce")// runs if lettuce is requested
        {
        setServoSpeed(S1, car, 20);
        while (SensorValue[S2] != BLUECOLOR)// waits for the colour sensor
//to detect blue.
        {}
        setServoSpeed(S1, car, 0);
        my_message = 2;
                sendMessage(my_message); // sends message to Slave NXT
                wait1Msec(3000);
                NotTwoBlue = false; // stores the information that the car
//has passed one blue strip.
        }
 
        if (item == "ketchup") // runs if ketchup is requested.
        {
                setServoSpeed(S1, car, 20);
        while(SensorValue[S2] != BLUECOLOR)//waits for the colour blue to
//be detected
        {}
                if(NotTwoBlue) // runs if lettuce was selected
                {
                        wait1Msec(1000);
                        while(SensorValue[S2] != BLUECOLOR) // waits for a
//second blue strip.
                                {}
                }
        setServoSpeed(S1, car, 0);
        my_message = 4;
                sendMessage(my_message); // sends a message to Slave NXT
                wait1Msec(3000);
        }
}
 
void getBread() // written by Cameron Blair
/*
This function is responsible for placing a slice of bread on the plate
using the servo claw.
*/
{
        int gripperArm = 1; // initializes the arm
        int gripper = 3; // initializes the claw
        setGripperPosition(S1, gripper, 50); // opens the claw
        wait1Msec(500);
        setServoPosition(S1, gripperArm, -40); // rotates the claw to the
//bread
        wait1Msec(500);
        setGripperPosition(S1, gripper, 12); // closes the claw on the\
//bread
        wait1Msec(500);
        setServoPosition(S1, gripperArm, -90); // rotates the claw with
//the bread to the plate
        wait1Msec(500);
        setGripperPosition(S1, gripper, 50); // opens the claw
        wait1Msec(200);
        setGripperPosition(S1, gripper, 0); // closes the claw
        wait1Msec(700);
        setServoPosition(S1, gripperArm, 70); // rotates the claw back to
//the front
}
 
void finish() // written by Eric Tweedle
/*
This function is to be run once all the toppings are recieved. It will bring
the car back to the start and ensure the costumer gets their sandwich.
*/
{
        int car = 2; // initializes the driving motor
        setServoSpeed(S1, car, -20); // drives the car in reverse
        while (SensorValue[S4] == 0) // waits for the rear touch sensor to
//be hit.
                {}
        setServoSpeed(S1, car, 0); // stops the car
        wait1Msec(1000);
        int my_message = 9;
        sendMessage(my_message);
        clearTimer(T1); // resets a timer
        while (time1[T1] < 10000) // runs for 10 seconds
        {
                displayString(3, "Sandwich is");
                displayString(4, "ready!");
                // below: plays a sound to the user to inform them that  
//their sandwich is ready
                playSound(soundFastUpwardTones);
                wait1Msec(500);
                playSound(soundDownwardTones);
                wait1Msec(3000);
        }
        eraseDisplay();
        displayString(3, "Enjoy!");
        wait1Msec(5000);
}
 
task main()// Written by Eric Tweedle
{
int gripperArm = 1; // initializes arm rotation motor
int car = 2; // initialized driving motor
setServoSpeed(S1, car, 0);
SensorType[S1] = sensorI2CCustom9V; // Initializes the connection to the
//TETRIX servo
SensorType[S2] = sensorColorNxtFULL; // Initializes colour sensor
SensorType[S3] = sensorTouch; // Initializes touch sensor that detects if
//a plate is on the car.
SensorType[S4] = sensorTouch; // Initializes touch sensor on the rear of
//the car.
 
setServoPosition(S1, gripperArm, 90); // sets the arm to the starting 
//position.
 
int PreSandwich = 0; // sets and integer that is changed by the Preselect
//function.
 
while (SensorValue[S3] == 0) // runs until a plate is placed on the car
{
        displayString(1, "Place Down");
        displayString(2, "Plate");
}
eraseDisplay();
PreSandwich = preselect(); // runs the preselct function
 
if (PreSandwich == 1)//cheese sandwich
{
        sandwich1.cheese = true;
        sandwich1.meat = false;
        sandwich1.lettuce = false;
        sandwich1.ketchup = false;
}
if(PreSandwich == 2)//Vegan sandwich
{
        sandwich1.lettuce = true;
        sandwich1.ketchup = true;
        sandwich1.meat = false;
        sandwich1.cheese = false;
}
 
getBread(); // runs the getBread function
string s = " ";
 
if (sandwich1.cheese) // gets cheese if cheese in the stuct is true
{
        s = "cheese";
        getItem(s);
}
wait1Msec(500);
if (sandwich1.lettuce) // gets lettuce if lettuce in the struct is true
{
        s = "lettuce";
        getItem(s);
}
wait1Msec(500);
if (sandwich1.meat) // gets meat if meat in the struct is true
{
        s = "meat";
        getItem(s);
}
wait1Msec(500);
if (sandwich1.ketchup) // gets ketchup if ketchup in the struct is true
{
        s = "ketchup";
        getItem(s);
}
wait1Msec(500);
finish(); // runs funish function
setServoSpeed(S1, car, 0); // stops all the car motor,used in the case it
//is not turned off.
}
/*
This code is the code that is present on the NXT controling 2 of the conveyer
belts and the condiment dispenser. The NXT will revcieve input from the NXT
running Main through bluetooth.
*/
const float PLUNGER = 1.2*180/PI*4; // Global constant for the length the condiment dispenser motor needs to travel
 
void RunConveyer(int num)//  Written by Morgan Yeung
/*
This function runs a conveyer belt or the condiment dispenser based on the message it receives from
the other NXT.
*/
{
        if (num == 3) // The integer message of 3 represents meat
        {
                motor(motorC) = -100;
                while   (nMotorEncoder(motorC) > -720) // runs for two
//full rotations
                {}
                motor(motorC) = 0;
        }
        if (num == 2)// The integer message of 2 represents lettuce
        {
                motor(motorB) = -100;
                while   (nMotorEncoder(motorB) > -720)
                {}
                motor(motorB) = 0;
        }
        if (num == 4) // The interger message of 4 represents condiments
        {
                motor(motorA) = -50;
                while (nMotorEncoder(motorA) < PLUNGER) // turns the
//amount defined by the constant PLUNGER
                {}
                motor(motorA) = 0;
        }
}
 
task main() // written by Ben Holmes
{
        ClearMessage(); // clears the message input. For when it is not
//cleared in the previous run
        int my_message = 0;
        while (my_message != 9) // runs a loop until it recieves a message
//of 9. Indicating it no longer needs to run
 {
        my_message = message; // sets a variable to the incoming message
        if (my_message != 0) // waits for a message to be sent and change 
//the variable my_message.
                RunConveyer(my_message); // which topping is decided by 
//run conveyer
        wait1Msec(500);
        ClearMessage(); // clears the message and waits for the next one.
        }
}
/*
This code is on the NXT that controls the first conveyer and the bread conveyer.
It does not connect to bluetooth so the commands are recieved through interactions
with touch sensors.
*/
task main() // Written by Cameron Blair
{
SensorType[S1] = sensorTouch; // the touch sensor that is hit by the arm
SensorType[S2] = sensorTouch; // the touch sensor that is bumbed by the
//touch sensor on the car
for (int i = 0; i < 2; i++) // runs for 2 cycles because it has up to 2 
//interactions per run.
{
        while (SensorValue[S1] == 0 && SensorValue[S2] == 0) // waits for 
//one of the sensors to be hit
                {}
        if (SensorValue[S1] == 1)//  if it is the first on it runs the 
//cheese conveyer belt
        {
                motor(motorA) = -100;
                wait1Msec(2000);
                motor(motorA) = 0;
        }
        if (SensorValue[S2] == 1) // if the bread bumber is the one that 
//hits.
        {
                motor(motorB) = -50;
                wait1Msec(1000);
                motor(motorB) = 0;
        }
        wait1Msec(3000); // waits 3 seconds before ending to allow all 
//processes to finish.
}
}

