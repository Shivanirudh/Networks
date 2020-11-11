BEGIN{
    recvdSize = 0
    startTime = 400
    stopTime = 0
    dropSize = 0
}
{
    event = $1
    time = $2
    node_id = $3
    pkt_size = $6
    level = $5

    if(level == "cbr" && (event == "+" || event =="s") ){
        if(time < startTime){
            startTime = time
        }
    }
    if(level == "cbr" && event == "r"){
        if(time > stopTime){
            stopTime = time
        }
        recvdSize++
    }
    if(level=="cbr" && event =="d"){
        dropSize++
    }
}
END{
    printf("Average Throughput[kbps] = %.2f\n", (recvdSize/(stopTime-startTime)));
    printf("Amount of data transferred in CBR[kb] = %.2f\n",recvdSize);
    printf("StartTime = %.2f\n", startTime);
    printf("StopTime = %.2f\n", stopTime)
    printf("Amount of data dropped in CBR[kb] = %.2f\n",dropSize);
}