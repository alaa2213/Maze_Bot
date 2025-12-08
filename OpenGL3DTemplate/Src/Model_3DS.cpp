void Model_3DS::MapNameChunkProcessor(long length, long findex, int matindex)
{
	char name[80];

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the name of the texture
	for (int i = 0; i < 80; i++)
	{
		name[i] = fgetc(bin3ds);
		if (name[i] == 0)
		{
			name[i] = NULL;
			break;
		}
	}

	// Load the name and indicate that the material has a texture
	char fullname[120]; // Increased buffer size
	sprintf_s(fullname, sizeof(fullname), "%s%s", path, name);
	
	// Attempt to load the texture (GLTexture::Load now handles .jpg -> .bmp conversion)
	Materials[matindex].tex.Load(fullname);

	// Check if the texture actually loaded. If not, it's not textured.
	if (Materials[matindex].tex.texture[0] == 0) {
		// The texture failed to load (e.g., file not found).
		// Mark this material as not textured so it can fall back to a solid color.
		Materials[matindex].textured = false;
	}
	else {
		// The texture loaded successfully.
		Materials[matindex].textured = true;
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}