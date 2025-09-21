
internal stream_chunk *
AppendChunk(stream* Stream, umw Size, void* Contents)
{
	stream_chunk* Chunk = PushStruct(Stream->Memory, stream_chunk);

	Chunk->Contents.Count = Size;
	Chunk->Contents.Data = (u8*)Contents;
	Chunk->Next = 0;

	Stream->Last = ((Stream->Last ? Stream->Last->Next : Stream->First) = Chunk);

	return (Chunk);
}