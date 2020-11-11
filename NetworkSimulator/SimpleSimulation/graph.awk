BEGIN{
    recvdSize = 0
    startTime = 400
    stopTime = 0
    dropSize = 0
    txSize = 0
    thruput = 0
    for(i=0;i<10;i++){
        arr[i] = 0;
    }
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
    if(level == "cbr" && (event == "r"){
        if(time > stopTime){
            stopTime = time
        }
        recvdSize++
        arr[node_id]++
        #printf("%.2f\t\t%.2f\n", time, recvdSize)
    }
    if(level=="cbr" && event =="d"){
        dropSize++
    }
}
END{
    for(i=0;i<10;i++){
        printf("Node %d %.2f\n", i, arr[i]);
    }
    #printf("Average Throughput[kbps] = %.2f\t\tStartTime = %.2f\tStopTime = %.2f\n", (recvdSize/(stopTime-startTime)), startTime, stopTime)
}