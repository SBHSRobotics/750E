//================================================================//
//  RobotC Music/Sound Library (NOTE: Will not compile directly)  //
//     Add converted RTTTLs here. Do not reference external files //
//================================================================//


void beep() {
  //        100 = Tempo
  //          5 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  PlayTone( 932,    27); wait1Msec(  75);  // Note(A#, Duration(32th))
  return;
}


void boop() {
  //        100 = Tempo
  //          6 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  PlayTone( 1047,  27); wait1Msec( 75);  // Note(F, Duration(32th))
  return;
}


task Dhoom()
{
  //        108 = Tempo
  //          5 = Default octave
  //      Whole = Default note length
  //        10% = Break between notes
  //
  PlayTone(  523,   25); wait1Msec( 278);  // Note(C, Duration(Half))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   25); wait1Msec( 278);  // Note(F, Duration(Half))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   13); wait1Msec( 139);  // Note(F, Duration(Quarter))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  622,   25); wait1Msec( 278);  // Note(D#, Duration(Half))
  PlayTone(  698,   13); wait1Msec( 139);  // Note(F, Duration(Quarter))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   25); wait1Msec( 278);  // Note(F, Duration(Half))
  PlayTone(  622,   13); wait1Msec( 139);  // Note(D#, Duration(Quarter))
  PlayTone(  587,   13); wait1Msec( 139);  // Note(D, Duration(Quarter))
  PlayTone(  523,   25); wait1Msec( 278);  // Note(C, Duration(Half))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   25); wait1Msec( 278);  // Note(F, Duration(Half))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   13); wait1Msec( 139);  // Note(F, Duration(Quarter))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  622,   25); wait1Msec( 278);  // Note(D#, Duration(Half))
  PlayTone(  698,   13); wait1Msec( 139);  // Note(F, Duration(Quarter))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   50); wait1Msec( 556);  // Note(F)
  PlayTone(  523,   25); wait1Msec( 278);  // Note(C, Duration(Half))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   25); wait1Msec( 278);  // Note(F, Duration(Half))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   13); wait1Msec( 139);  // Note(F, Duration(Quarter))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  622,   25); wait1Msec( 278);  // Note(D#, Duration(Half))
  PlayTone(  698,   13); wait1Msec( 139);  // Note(F, Duration(Quarter))
  PlayTone(  783,   13); wait1Msec( 139);  // Note(G, Duration(Quarter))
  PlayTone(  698,   25); wait1Msec( 278);  // Note(F, Duration(Half))
  PlayTone(  622,   13); wait1Msec( 139);  // Note(D#, Duration(Quarter))
  PlayTone(  587,   13); wait1Msec( 139);  // Note(D, Duration(Quarter))
  PlayTone(  523,   25); wait1Msec( 278);  // Note(C, Duration(Half))
  PlayTone(  554,   13); wait1Msec( 139);  // Note(C#, Duration(Quarter))
  PlayTone(  622,   13); wait1Msec( 139);  // Note(D#, Duration(Quarter))
  PlayTone(  698,   25); wait1Msec( 278);  // Note(F, Duration(Half))
  PlayTone(  622,   13); wait1Msec( 139);  // Note(D#, Duration(Quarter))
  PlayTone(  554,   13); wait1Msec( 139);  // Note(C#, Duration(Quarter))
  PlayTone(  523,   25); wait1Msec( 278);  // Note(C, Duration(Half))
  PlayTone(  554,   13); wait1Msec( 139);  // Note(C#, Duration(Quarter))
  PlayTone(  523,   25); wait1Msec( 278);  // Note(C, Duration(Half))
  PlayTone(  554,   13); wait1Msec( 139);  // Note(C#, Duration(Quarter))
  PlayTone(  523,   50); wait1Msec( 556);  // Note(C)
  StopTask(Dhoom);
}


task TakeOnMe()
{
  //        160 = Tempo
  //          4 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  //
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  587,   17); wait1Msec( 188);  // Note(D5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  493,   17); wait1Msec( 188);  // Note(B, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(  830,   17); wait1Msec( 188);  // Note(G#5, Duration(Eighth))
  PlayTone(  830,   17); wait1Msec( 188);  // Note(G#5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  987,   17); wait1Msec( 188);  // Note(B5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  587,   17); wait1Msec( 188);  // Note(D5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  587,   17); wait1Msec( 188);  // Note(D5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  493,   17); wait1Msec( 188);  // Note(B, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(  830,   17); wait1Msec( 188);  // Note(G#5, Duration(Eighth))
  PlayTone(  830,   17); wait1Msec( 188);  // Note(G#5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  987,   17); wait1Msec( 188);  // Note(B5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  880,   17); wait1Msec( 188);  // Note(A5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  587,   17); wait1Msec( 188);  // Note(D5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(    0,   17); wait1Msec( 188);  // Note(Rest, Duration(Eighth))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E5, Duration(Eighth))
  StopTask(TakeOnMe);
}


task PinkPanther()
{
  //        160 = Tempo
  //          5 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  //
  PlayTone(  622,   17); wait1Msec( 188);  // Note(D#, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E, Duration(Eighth))
  PlayTone(    0,   68); wait1Msec( 750);  // Note(Rest, Duration(Half))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#, Duration(Eighth))
  PlayTone(  783,   17); wait1Msec( 188);  // Note(G, Duration(Eighth))
  PlayTone(    0,   68); wait1Msec( 750);  // Note(Rest, Duration(Half))
  PlayTone(  622,   17); wait1Msec( 188);  // Note(D#, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E, Duration(Eighth))
  PlayTone(    0,    8); wait1Msec(  94);  // Note(Rest, Duration(16th))
  PlayTone(  739,   17); wait1Msec( 188);  // Note(F#, Duration(Eighth))
  PlayTone(  783,   17); wait1Msec( 188);  // Note(G, Duration(Eighth))
  PlayTone(    0,    8); wait1Msec(  94);  // Note(Rest, Duration(16th))
  PlayTone( 1046,   17); wait1Msec( 188);  // Note(C7, Duration(Eighth))
  PlayTone(  987,   17); wait1Msec( 188);  // Note(B, Duration(Eighth))
  PlayTone(    0,    8); wait1Msec(  94);  // Note(Rest, Duration(16th))
  PlayTone(  622,   17); wait1Msec( 188);  // Note(D#, Duration(Eighth))
  PlayTone(  659,   17); wait1Msec( 188);  // Note(E, Duration(Eighth))
  PlayTone(    0,    8); wait1Msec(  94);  // Note(Rest, Duration(16th))
  PlayTone(  987,   17); wait1Msec( 188);  // Note(B, Duration(Eighth))
  PlayTone(  932,   68); wait1Msec( 750);  // Note(A#, Duration(Half))
  PlayTone(    0,   68); wait1Msec( 750);  // Note(Rest, Duration(Half))
  PlayTone(  880,    8); wait1Msec(  94);  // Note(A, Duration(16th))
  PlayTone(  783,    8); wait1Msec(  94);  // Note(G, Duration(16th))
  PlayTone(  659,    8); wait1Msec(  94);  // Note(E, Duration(16th))
  PlayTone(  587,    8); wait1Msec(  94);  // Note(D, Duration(16th))
  PlayTone(  659,   68); wait1Msec( 750);  // Note(E, Duration(Half))
  StopTask(PinkPanther);
}


task HotlineBling()
{
  //        130 = Tempo
  //          6 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  //
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone( 1047,   21); wait1Msec( 231);  // Note(F, Duration(Eighth))
  PlayTone(  988,   21); wait1Msec( 231);  // Note(E, Duration(Eighth))
  PlayTone(  880,   21); wait1Msec( 231);  // Note(D, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone(  988,   42); wait1Msec( 462);  // Note(E)
  PlayTone(  784,   42); wait1Msec( 462);  // Note(C)
  PlayTone(    0,   42); wait1Msec( 462);  // Note(Rest)
  PlayTone(    0,   42); wait1Msec( 462);  // Note(Rest)
  PlayTone(    0,   42); wait1Msec( 462);  // Note(Rest)
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone( 1047,   21); wait1Msec( 231);  // Note(F, Duration(Eighth))
  PlayTone(  988,   21); wait1Msec( 231);  // Note(E, Duration(Eighth))
  PlayTone(  880,   21); wait1Msec( 231);  // Note(D, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone(  988,   83); wait1Msec( 923);  // Note(E, Duration(Half))
  PlayTone(  784,   83); wait1Msec( 923);  // Note(C, Duration(Half))
  PlayTone(  659,   83); wait1Msec( 923);  // Note(E5, Duration(Half))
  PlayTone( 1047,   21); wait1Msec( 231);  // Note(F, Duration(Eighth))
  PlayTone(  988,   21); wait1Msec( 231);  // Note(E, Duration(Eighth))
  PlayTone(  880,   21); wait1Msec( 231);  // Note(D, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone(  988,   42); wait1Msec( 462);  // Note(E)
  PlayTone(  784,   42); wait1Msec( 462);  // Note(C)
  PlayTone(    0,   42); wait1Msec( 462);  // Note(Rest)
  PlayTone(    0,   42); wait1Msec( 462);  // Note(Rest)
  PlayTone(    0,   42); wait1Msec( 462);  // Note(Rest)
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone( 1047,   21); wait1Msec( 231);  // Note(F, Duration(Eighth))
  PlayTone(  988,   21); wait1Msec( 231);  // Note(E, Duration(Eighth))
  PlayTone(  880,   21); wait1Msec( 231);  // Note(D, Duration(Eighth))
  PlayTone(  784,   21); wait1Msec( 231);  // Note(C, Duration(Eighth))
  PlayTone(  988,   83); wait1Msec( 923);  // Note(E, Duration(Half))
  PlayTone(  784,   83); wait1Msec( 923);  // Note(C, Duration(Half))
  PlayTone(  659,   83); wait1Msec( 923);  // Note(E5, Duration(Half))
  StopTask(HotlineBling);
}

task JohnCena(){
	//        130 = Tempo
  //          6 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  //
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone( 1175,   42); wait1Msec( 462);  // Note(G)
  PlayTone( 1320,   21); wait1Msec( 231);  // Note(A, Duration(Eighth))
  PlayTone( 1047,   42); wait1Msec( 462);  // Note(F)
  PlayTone( 1175,   83); wait1Msec( 923);  // Note(G, Duration(Half))
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone( 1398,   42); wait1Msec( 462);  // Note(A#)
  PlayTone( 1320,   21); wait1Msec( 231);  // Note(A, Duration(Eighth))
  PlayTone( 1047,   42); wait1Msec( 462);  // Note(F)
  PlayTone( 1175,   83); wait1Msec( 923);  // Note(G, Duration(Half))
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone( 1175,   42); wait1Msec( 462);  // Note(G)
  PlayTone( 1320,   21); wait1Msec( 231);  // Note(A, Duration(Eighth))
  PlayTone( 1047,   42); wait1Msec( 462);  // Note(F)
  PlayTone( 1175,   83); wait1Msec( 923);  // Note(G, Duration(Half))
  PlayTone(    0,   21); wait1Msec( 231);  // Note(Rest, Duration(Eighth))
  PlayTone( 1398,   42); wait1Msec( 462);  // Note(A#)
  PlayTone( 1320,   21); wait1Msec( 231);  // Note(A, Duration(Eighth))
  PlayTone( 1047,   42); wait1Msec( 462);  // Note(F)
  PlayTone( 1175,   83); wait1Msec( 923);  // Note(G, Duration(Half))
  StopTask(JohnCena);

}


void success() {
	//TODO: Kickoff sound
}
