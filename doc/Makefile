all: classes.puml

classes.puml: ../include/parser.h ../Lab2/src/table_parser.h ../Lab4/src/rule_parser.h
	hpp2plantuml $(patsubst %,-i %,$^) -o $@

