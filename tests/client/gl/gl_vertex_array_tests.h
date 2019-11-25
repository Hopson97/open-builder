#pragma once

#include <catch2/catch.hpp>
#include <client/gl/gl_vertex_array.h>

TEST_CASE("Vertex array is able to created, modified, and cleanly destroyed")
{
    VertexArrayContainer container = createVertexArray();

    REQUIRE(container.object.handle != 0);

	destroyVertexArray(&container);

	REQUIRE(container.object.handle == 0);

    std::vector<GLfloat> buffer = {0, 1, 2, 3};
    std::vector<GLuint> indices = {1, 2, 3, 4};

    SECTION("Vertex buffer can be added to the vertex array object")
    {
        VertexArrayContainer container = createVertexArray();
        addVertexBuffer(&container, 2, buffer, DrawStyle::Static,
                        GLType::Float);

		REQUIRE(container.bufferObjects.size() == 1);

		destroyVertexArray(&container);
	}

    SECTION("Vertex buffer and indices can be added to the VAO")
	{
        VertexArrayContainer container = createVertexArray();
        addVertexBuffer(&container, 2, buffer, DrawStyle::Static,
                        GLType::Float);
        addIndexBuffer(&container, indices);

        REQUIRE(container.bufferObjects.size() == 2);
        REQUIRE(container.indicesCount == indices.size());

        destroyVertexArray(&container);
	}

    SECTION("Vertex buffer container can be destroyed")
	{
	    VertexArrayContainer container = createVertexArray();
	    addVertexBuffer(&container, 2, buffer, DrawStyle::Static,
	                    GLType::Float);
	    addIndexBuffer(&container, indices);

		destroyVertexArray(&container);
	
	    REQUIRE(container.bufferObjects.size() == 0);
	    REQUIRE(container.indicesCount == 0);
		REQUIRE(container.object.handle == 0);
	}
}