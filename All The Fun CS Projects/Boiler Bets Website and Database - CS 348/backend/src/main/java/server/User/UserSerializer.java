package server.User;

import java.io.IOException;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;

public class UserSerializer extends StdSerializer<User> {


	private static final long serialVersionUID = 1L;

	public UserSerializer() {
		this(null);
	}

	public UserSerializer(Class<User> t) {
		super(t);
	}

	@Override
	public void serialize(User value, JsonGenerator gen, SerializerProvider provider) throws IOException {
		gen.writeStartObject();
		gen.writeStringField("userName", value.username);
		gen.writeStringField("password", value.password);
		gen.writeStringField("name", value.name);
		gen.writeStringField("email", value.password);
		
		gen.writeEndObject();
	}

}