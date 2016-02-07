#if defined _smjansson_newdecls_included
	#endinput
#endif

#define _smjansson_newdecls_included

#if !defined _smjansson_included
	#pragma newdecls optional
	#include <smjansson>
#endif

#pragma newdecls required

enum JSONObjectUpdateType {
	Update_All = 0,
	Update_Existing,
	Update_Missing
};

enum JSONType {
	JSONType_Object,
	JSONType_Array,
	JSONType_String,
	JSONType_Integer,
	JSONType_Real,
	JSONType_True,
	JSONType_False,
	JSONType_Null
};

/**
 * Base methodmap for all of smjansson's handles
 */
methodmap JSONValue < Handle {
	property JSONType Type {
		public get() {
			view_as<JSONType>(json_typeof(this));
		}
	}
	property any Value {
		// not implemented
	}
	
	public bool Equals(JSONValue other) {
		return json_equal(this, other);
	}
	public JSONValue Copy(bool deep = false) {
		if (deep) {
			return view_as<JSONValue>(json_deep_copy(this));
		} else {
			return view_as<JSONValue>(json_copy(this));
		}
	}
	public void TypeToString(char[] buffer, int maxlength) {
		if (!Stringify_json_type(view_as<json_type>(this.Type), buffer, maxlength)) {
			ThrowError("Attempting to get type of a non-SMJansson handle");
		}
	}
}

methodmap JSONBoolean < JSONValue {
	public JSONBoolean(bool value) {
		return view_as<JSONBoolean>(value ? json_true() : json_false());
	}
	property bool Value {
		public get() {
			if (this.Type == JSONType_True) {
				return true;
			} else if (this.Type == JSONType_False) {
				return false;
			}
			ThrowError("Not a JSONBoolean value");
			return false;
		}
	}
}

methodmap JSONNull < JSONValue {
	public JSONNull() {
		return view_as<JSONNull>(json_null());
	}
}

methodmap JSONString < JSONValue {
	public JSONString(const char[] value) {
		return view_as<JSONString>(json_string(value));
	}
	
	public static JSONString Format(int bufferSize = 4096, const char[] format, any ...) {
		char[] buffer = new char[bufferSize];
		VFormat(buffer, bufferSize, format, 3);
		return new JSONString(buffer);
	}
	
	public int GetString(char[] buffer, int length) {
		return json_string_value(this, buffer, length);
	}
	
	public bool SetString(char[] value) {
		return json_string_set(this, value);
	}
}

/**
 * Base map inherited by JSONInteger and JSONFloat to get the value of either, cast to float.
 */
methodmap JSONNumber < JSONValue {
	public float FloatValue() {
		return json_number_value(this);
	}
}

methodmap JSONInteger < JSONNumber {
	public JSONInteger(int value) {
		return view_as<JSONInteger>(json_integer(value));
	}
	property int Value {
		public get() {
			return json_integer_value(this);
		}
		public set(int value) {
			json_integer_set(this, value);
		}
	}
}

methodmap JSONFloat < JSONNumber {
	public JSONFloat(float value) {
		return view_as<JSONFloat>(json_real(value));
	}
	property float Value {
		public get() {
			return json_real_value(this);
		}
		public set(float value) {
			json_real_set(this, value);
		}
	}
}

/**
 * Provides a few functions for valid root nodes (JSONArray and JSONObject)
 */
methodmap JSONRootNode < JSONValue {
	/**
	 * Dumps the contents of the given root node to a string.
	 * 
	 * @return Number of characters written
	 */
	public int ToString(char[] buffer, int maxlength, int indentWidth = 4,
			bool asciiOnly = false, bool sortKeys = false, bool preserveOrder = false) {
		return json_dump(this, buffer, maxlength, indentWidth, asciiOnly, sortKeys,
				preserveOrder);
	}
	
	/**
	 * @return True on success
	 */
	public bool ToFile(const char[] filePath, int indentWidth = 4, bool asciiOnly = false,
			bool sortKeys = false, bool preserveOrder = false) {
		return json_dump_file(this, filePath, indentWidth, asciiOnly, sortKeys, preserveOrder);
	}
	
	public static JSONRootNode Pack(const char[] packString, ArrayList params) {
		return json_pack(packString, params);
	}
}

methodmap JSONArray < JSONRootNode {
	public JSONArray() {
		return view_as<JSONArray>(json_array());
	}
	
	property int Length {
		public get() {
			if (this.Type != JSONType_Array) {
				ThrowError("Handle is not an array");
			}
			return json_array_size(this);
		}
	}
	
	public JSONValue Get(int index) {
		return view_as<JSONValue>(json_array_get(this, index));
	}
	
	public bool Set(int index, JSONValue value, bool autoClose = true) {
		if (autoClose) {
			return json_array_set_new(this, index, value);
		}
		return json_array_set(this, index, value);
	}
	
	public bool Append(JSONValue value, bool autoClose = true) {
		if (autoClose) {
			return json_array_append_new(this, value);
		}
		return json_array_append(this, value);
	}
	
	public bool Insert(int index, JSONValue value, bool autoClose = true) {
		if (autoClose) {
			return json_array_insert_new(this, index, value);
		}
		return json_array_insert(this, index, value);
	}
	
	public bool Remove(int index) {
		return json_array_remove(this, index);
	}
	
	public bool Clear() {
		return json_array_clear(this);
	}
	
	public bool Extend(JSONArray other) {
		return json_array_extend(this, other);
	}
	
	/**
	 * Utility functions
	 */
	
	/* Booleans */
	
	public bool GetBool(int index, bool defValue = false) {
		JSONValue value = this.Get(index);
		JSONType type = value.Type;
		delete value;
		
		switch (type) {
			case JSONType_True: { return true; }
			case JSONType_False: { return false; }
		}
		return defValue;
	}
	
	public bool SetBool(int index, bool value) {
		return this.Set(index, new JSONBoolean(value));
	}
	
	public bool AppendBool(bool value) {
		return this.Append(new JSONBoolean(value));
	}
	
	public bool InsertBool(int index, bool value) {
		return this.Insert(index, new JSONBoolean(value));
	}
	
	/* Floats */
	
	public float GetFloat(int index, float defValue = 0.0) {
		JSONValue value = this.Get(index);
		float result = (json_is_number(value) ?
				(view_as<JSONNumber>(value)).FloatValue() : defValue);
		delete value;
		
		return result;
	}
	
	public bool SetFloat(int index, float value) {
		return this.Set(index, new JSONFloat(value));
	}
	
	public bool AppendFloat(float value) {
		return this.Append(new JSONFloat(value));
	}
	
	public bool InsertFloat(int index, float value) {
		return this.Insert(index, new JSONFloat(value));
	}
	
	/* Integers */
	
	public int GetInt(int index, int defValue = 0) {
		JSONValue value = this.Get(index);
		int result = value.Type == JSONType_Integer ? (view_as<JSONInteger>(value)).Value : 0;
		delete value;
		
		return result;
	}
	
	public bool SetInt(int index, int value) {
		return this.Set(index, new JSONInteger(value));
	}
	
	public bool AppendInt(int value) {
		return this.Append(new JSONInteger(value));
	}
	
	public bool InsertInt(int index, int value) {
		return this.Insert(index, new JSONInteger(value));
	}
	
	/* Strings */
	
	public int GetString(int index, char[] buffer, int maxlength) {
		JSONValue value = this.Get(index);
		
		int result;
		if (value.Type == JSONType_String) {
			result = (view_as<JSONString>(value)).GetString(buffer, maxlength);
		}
		delete value;
		return result;
	}
	
	public bool SetString(int index, const char[] value) {
		return this.Set(index, new JSONString(value));
	}
	
	public bool AppendString(const char[] value) {
		return this.Append(new JSONString(value));
	}
	
	public bool InsertString(int index, const char[] value) {
		return this.Insert(index, new JSONString(value));
	}
	
	public static JSONArray Pack(const char[] packString, ArrayList params) {
		int length = strlen(packString);
		if (packString[0] == '[' && packString[length - 1] == ']') {
			return view_as<JSONArray>(JSONRootNode.Pack(packString, params));
		} else {
			ThrowError("Pack string '%s' is not appropriate for creating a JSONArray",
					packString);
			return null;
		}
	}
}

methodmap JSONObject < JSONRootNode {
	public JSONObject() {
		return view_as<JSONObject>(json_object());
	}
	
	property int Size {
		public get() {
			return json_object_size(this);
		}
	}
	
	public JSONValue Get(const char[] key) {
		return view_as<JSONValue>(json_object_get(this, key));
	}
	
	public bool Set(const char[] key, JSONValue value, bool autoClose = true) {
		if (autoClose) {
			return json_object_set_new(this, key, value);
		}
		return json_object_set(this, key, value);
	}
	
	/**
	 * @return true on success
	 */
	public bool Remove(const char[] key) {
		return json_object_del(this, key);
	}
	
	public bool Clear() {
		return json_object_clear(this);
	}
	
	public bool Update(JSONObject other, JSONObjectUpdateType updateType = Update_All) {
		switch (updateType) {
			case Update_Existing: {
				return json_object_update_existing(this, other);
			}
			case Update_Missing: {
				return json_object_update_missing(this, other);
			}
		}
		return json_object_update(this, other);
	}
	
	/**
	 * Utility functions
	 */
	
	public bool GetBool(const char[] key, bool defValue = false) {
		JSONValue value = this.Get(key);
		JSONType type = value.Type;
		delete value;
		
		switch (type) {
			case JSONType_True: { return true; }
			case JSONType_False: { return false; }
		}
		return defValue;
	}
	
	public bool SetBool(const char[] key, bool value) {
		return this.Set(key, new JSONBoolean(value));
	}
	
	/* Floats */
	
	public float GetFloat(const char[] key, float defValue = 0.0) {
		JSONValue value = this.Get(key);
		float result = (json_is_number(value) ?
				(view_as<JSONNumber>(value)).FloatValue() : defValue);
		delete value;
		
		return result;
	}
	
	public bool SetFloat(const char[] key, float value) {
		return this.Set(key, new JSONFloat(value));
	}
	
	/* Integers */
	
	public int GetInt(const char[] key, int defValue = 0) {
		JSONValue value = this.Get(key);
		int result = value.Type == JSONType_Integer ?
				(view_as<JSONInteger>(value)).Value : defValue;
		delete value;
		
		return result;
	}
	
	public bool SetInt(const char[] key, int value) {
		return this.Set(key, new JSONInteger(value));
	}
	
	/* Strings */
	
	public int GetString(const char[] key, char[] buffer, int maxlength) {
		JSONValue value = this.Get(key);
		
		int result;
		if (value.Type == JSONType_String) {
			result = (view_as<JSONString>(value)).GetString(buffer, maxlength);
		}
		delete value;
		return result;
	}
	
	public bool SetString(const char[] key, const char[] value) {
		return this.Set(key, new JSONString(value));
	}
	
	public static JSONObject Pack(const char[] packString, ArrayList params) {
		int length = strlen(packString);
		if (packString[0] == '{' && packString[length - 1] == '}') {
			return view_as<JSONObject>(JSONRootNode.Pack(packString, params));
		} else {
			ThrowError("Pack string '%s' is not appropriate for creating a JSONObject",
					packString);
			return null;
		}
	}
}

methodmap JSONObjectIterator < Handle {
	public static JSONObjectIterator From(JSONObject obj, const char[] key = "") {
		if (strlen(key) > 0) {
			return view_as<JSONObjectIterator>(json_object_iter_at(obj, key));
		}
		return view_as<JSONObjectIterator>(json_object_iter(obj));
	}
	
	public JSONObjectIterator Next(JSONObject obj) {
		return view_as<JSONObjectIterator>(json_object_iter_next(obj, this));
	}
	
	public int GetKey(char[] buffer, int maxlength) {
		return json_object_iter_key(this, buffer, maxlength);
	}
	
	public JSONValue GetValue() {
		return view_as<JSONValue>(json_object_iter_value(this));
	}
	
	public bool SetValue(JSONObject obj, JSONValue value, bool autoclose = true) {
		if (autoclose) {
			return json_object_iter_set_new(obj, this, value);
		}
		return json_object_iter_set(obj, this, value);
	}
}