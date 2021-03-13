CXX=g++
XXFLAGS=-Wall -MMD
EXEC=chess
OBJECTS=main.o window.o board.o piece.o coord.o king.o knight.o bishop.o queen.o pawn.o rook.o ai.o
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${XXFLAGS} ${OBJECTS} -o ${EXEC} -lX11
-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC}
