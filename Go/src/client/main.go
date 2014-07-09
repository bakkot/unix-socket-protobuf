package main

import (
	"log"
	"net"
	//"fmt"
	//"bytes"
	"encoding/binary"

	"code.google.com/p/goprotobuf/proto"
	"example_mess"
)

var _ = net.Dial
//var _ = bytes.Buffer

func main() {

	socket, err := net.Dial("unix", "socket")
	if err!= nil {
		log.Fatal("Failed to connect to socket: ", err)
	}
	
	defer socket.Close()

	test := &example_mess.Boring {
		Cont: proto.String("Greets from Go"),
	}
	serialized, err := proto.Marshal(test)
	if err != nil {
		log.Fatal("marshaling error: ", err)
	}
	
	
	//buf := new(bytes.Buffer)
	println(len(serialized))
	//err = binary.Write(buf, binary.BigEndian, int32(1337))
	//fmt.Printf("% d", buf.Bytes())
	
	// Go is unique in that, thanks to its interfaces, it's practical to encode directly to the socket
	err = binary.Write(socket, binary.BigEndian, int32(len(serialized)))
	if err != nil {
		log.Fatal("failed writing to socket: ", err)
	}
	
	socket.Write(serialized)
}
