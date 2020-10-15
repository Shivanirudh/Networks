set ns [new Simulator]

$ns color 1 blue
$ns color 2 red

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

set f [open out.tr w]
$ns trace-all $f
set nf [open out.nam w]
$ns namtrace-all $nf

$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 100ms DropTail
$ns simplex-link $n3 $n2 0.3Mb 100ms DropTail
$ns duplex-link $n3 $n4 0.5Mb 40ms DropTail
$ns duplex-link $n3 $n5 0.5Mb 40ms DropTail

$ns duplex-link-op $n0 $n2 orient right-up
$ns duplex-link-op $n1 $n2 orient right-down
$ns simplex-link-op $n2 $n3 orient right
$ns simplex-link-op $n3 $n2 orient left
$ns duplex-link-op $n3 $n4 orient right-up
$ns duplex-link-op $n3 $n5 orient right-down

$ns queue-limit $n2 $n3 10

set tcp [new Agent/TCP/Newreno]
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
$tcp set window_ 8000
$tcp set packetSize_ 512
$tcp set class_ 2
set ftp [new Application/FTP]
$ftp attach-agent $tcp

set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1
$udp1 set class_ 1
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set type_ CBR
$cbr1 set packet_size_ 1024
$cbr1 set rate_ 0.01mb
$cbr1 set random_ false

set udp2 [new Agent/UDP]
$ns attach-agent $n5 $udp2
$udp2 set class_ 1
set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $udp2
$cbr2 set type_ CBR
$cbr2 set packet_size_ 1024
$cbr2 set rate_ 0.01mb
$cbr2 set random_ false

set null1 [new Agent/Null]
$ns attach-agent $n1 $null1

set null2 [new Agent/Null]
$ns attach-agent $n5 $null2

$ns connect $udp1 $null1
$ns connect $udp2 $null2

$ns at 0.1 "$cbr1 start"
$ns at 0.2 "$cbr2 start"
$ns at 1.0 "$ftp start"
$ns at 4.5 "$ftp start"
$ns at 5.0 "$cbr2 stop"
$ns at 5.1 "$cbr1 stop"

$ns at 7.0 "finish"

puts [$cbr1 set packetSize_]
puts [$cbr2 set interval_]

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