#define setMotor startMotor
#define delay wait

task main()
{
setMotor(1,127);
setMotor(2,127);
setMotor(3,-127);
setMotor(4,-127);
setMotor(8,-127);
setMotor(9,-127);
delay(50);
}
