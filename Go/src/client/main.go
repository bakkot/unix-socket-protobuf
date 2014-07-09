package main

import (
	"log"

	"code.google.com/p/goprotobuf/proto"
	"example_mess"
)

func main() {
	test := &example_mess.Boring {
		Cont: proto.String("wew"),
	}
	_, err := proto.Marshal(test)
	if err != nil {
		log.Fatal("marshaling error: ", err)
	}
}
