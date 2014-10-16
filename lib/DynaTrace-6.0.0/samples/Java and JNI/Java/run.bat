set CLASSPATH=src/;../../../adk/lib/com.dynatrace.adk.jar
java -agentpath:"%DT_AGENTLIBRARY%"=name=javaJNI -Djava.library.path=../Native/Debug com.dynatrace.samples.HelloWorldJNI

REM When using java 1.4 use -Xrundtagent option instead of -agentpath:
REM set PATH=%PATH%;../../../agent/lib
REM java -Xrundtagent:name=javaJNI,server=localhost -Djava.library.path=../Native/Debug com.dynatrace.samples.HelloWorldJNI
