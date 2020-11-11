#ii. Use simulator to analyse the bottleneck problem in TCP and UDP.Assume Blue for TCP and Red for UDP.

set ns [new Simulator]

$ns color 1 blue
$ns color 2 red

set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]

set f [open out.tr w]
$ns trace-all $f
set nf [open out.nam w]
$ns namtrace-all $nf

$ns duplex-link $n1 $n3 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 2Mb 10ms DropTail
$ns simplex-link $n3 $n4 0.3Mb 100ms DropTail
$ns simplex-link $n4 $n3 0.3Mb 100ms DropTail
$ns duplex-link $n4 $n5 0.5Mb 40ms DropTail
$ns duplex-link $n4 $n6 0.5Mb 40ms DropTail

$ns duplex-link-op $n1 $n3 orient right-down
$ns duplex-link-op $n2 $n3 orient right-up
$ns simplex-link-op $n3 $n4 orient right
$ns simplex-link-op $n4 $n3 orient left
$ns duplex-link-op $n4 $n5 orient right-up
$ns duplex-link-op $n4 $n6 orient right-down

$ns queue-limit $n3 $n4 10

set tcp [new Agent/TCP/Newreno]
$ns attach-agent $n2 $tcp
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n6 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
$tcp set window_ 8000
$tcp set packetSize_ 512
$tcp set class_ 1

set ftp [new Application/FTP]
$ftp attach-agent $tcp

set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
$udp set class_ 2

set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set type_ CBR
$cbr set packet_size_ 1024
$cbr set rate_ 0.01mb
$cbr set random_ false

set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp $null

$ns at 0.1 "$cbr start"
$ns at 0.5 "$ftp start"
$ns at 4.7 "$ftp stop"
$ns at 5.0 "$cbr stop"

$ns at 7.0 "finish"

proc finish {} {
	global ns f nf
	$ns flush-trace
	close $f
	close $nf

	puts "running nam..."
	exec nam out.nam & 
	exit 0
}

$ns run