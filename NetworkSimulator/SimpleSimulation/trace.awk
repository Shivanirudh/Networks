BEGIN{
    recvdSize = 0
    startTime = 400
    stopTime = 0
    dropSize = 0
    txSize = 0
    thruput = 0
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
        txSize++
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
    printf("Average Throughput[kbps] = %.2f\t\tStartTime = %.2f\tStopTime = %.2f\n", (recvdSize/(stopTime-startTime)), startTime, stopTime)
}