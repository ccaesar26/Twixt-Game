#include "Peg.h"

IPiecePtr IPiece::Produce(EColor color)
{
	return std::make_shared<Peg>(color);
}