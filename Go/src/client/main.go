package main

import (
	"encoding/binary"
	"log"
	"net"

	"code.google.com/p/goprotobuf/proto"
	"example_mess"
)

func passOrDie(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func main() {
	socket, err := net.Dial("unix", "socket")
	passOrDie(err)
	defer socket.Close()

	msg := &example_mess.Boring{
		Cont: proto.String("Greets from Go"),
	}

	serialized, err := proto.Marshal(msg)
	passOrDie(err)

	// Go is unique in that, thanks to its interfaces, it's practical to encode directly to the socket
	err = binary.Write(socket, binary.BigEndian, int32(len(serialized)))
	passOrDie(err)

	socket.Write(serialized)
}
