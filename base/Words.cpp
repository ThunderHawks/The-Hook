/*void initText2D(const char * texturePath);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();


std::vector<glm::vec2> vertices;
std::vector<glm::vec2> UVs;

void printText2D(const char * text, int x, int y, int size) {
	char character;
	float uv_x, uv_y;

	for ( unsigned int i=0 ; i<length ; i++ ){
		character = text[i] - 32;
		uv_x = (character%10)/10.0f;
		uv_y = (character/10)/10.0f;

		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , 1.0f - uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/10.0f, 1.0f - uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/10.0f, 1.0f - (uv_y + 1.0f/10.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , 1.0f - (uv_y + 1.0f/10.0f) );
	 
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );
	 
		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
}
*/