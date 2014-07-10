package main

import (
	"encoding/binary"
	"log"
	"net"
	"syscall"

	"code.google.com/p/goprotobuf/proto"
	"example_mess"
)

func passOrDie(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func handler(connection net.Conn) {
	var message_length int32

	// encoding/binary is quite clever! Notice that we're reading from the socket and encoding directly into the int32 here. Exactly 4 bytes (the size of message_length) will be read.
	err := binary.Read(connection, binary.BigEndian, &message_length)
	passOrDie(err)

	println(message_length)

	msg := new(example_mess.Boring)

	buf := make([]byte, message_length)
	err = binary.Read(connection, binary.BigEndian, buf)
	passOrDie(err)
	err = proto.Unmarshal(buf, msg)
	passOrDie(err)

	println("Message: ", msg.GetCont())

}

func main() {
	syscall.Unlink("socket")
	listener, err := net.Listen("unix", "socket")
	passOrDie(err)

	for {
		connection, err := listener.Accept()
		passOrDie(err)

		go handler(connection)
	}

}
