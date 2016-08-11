#if defined _msgpack_newdecls_included
	#endinput
#endif

#define _msgpack_newdecls_included

#if !defined _msgpack_included
	#pragma newdecls optional
	#include <msgpack>
#endif
#pragma newdecls required

methodmap MsgPack < Handle {
	public MsgPack() {
		return view_as<MsgPack>(MsgPack_NewPack());
	}
	public void PackInt(int value) {
		MsgPack_PackInt(this, value);
	}
	public void PackFloat(float value) {
		MsgPack_PackFloat(this, value);
	}
	public void PackNil() {
		MsgPack_PackNil(this);
	}
	public void PackBool(bool value) {
		if (value) {
			MsgPack_PackTrue(this);
		} else {
			MsgPack_PackFalse(this);
		}
	}
	public void PackArray(int size) {
		MsgPack_PackArray(this, size);
	}
	/**
	 * Packs a map.
	 * Each map index contains a key and a value.
	 */
	public void PackMap(int size) {
		MsgPack_PackMap(this, size);
	}
	public void PackBytes(const char[] bytes, int length) {
		MsgPack_PackRaw(this, bytes, length);
	}
	public void PackString(const char[] string) {
		MsgPack_PackString(this, string);
	}
	property int Length {
		public get() {
			return MsgPack_GetPackSize(this);
		}
	}
	public void GetBuffer(const char[] buffer, int length) {
		MsgPack_GetPackBuffer(this, buffer, length);
	}
}

methodmap MsgUnpack < Handle {
	public MsgUnpack(const char[] buffer, int length, int &offset=0, bool bSave = true) {
		return view_as<MsgUnpack>(MsgPack_NewUnpack(buffer, length, offset, bSave));
	}
	public MsgPack_Type UnpackType() {
		return MsgPack_UnpackType(this);
	}
	public int UnpackCount() {
		return MsgPack_UnpackCount(this);
	}
	public bool UnpackBool() {
		return MsgPack_UnpackBool(this);
	}
	public int UnpackInt() {
		return MsgPack_UnpackInt(this);
	}
	public float UnpackFloat() {
		return MsgPack_UnpackFloat(this);
	}
	public int UnpackBytes(char[] buffer, int length) {
		return MsgPack_UnpackRaw(this, buffer, length);
	}
	public int UnpackString(char[] buffer, int length) {
		return MsgPack_UnpackString(this, buffer, length);
	}
	public MsgUnpack UnpackArray(int index) {
		return view_as<MsgUnpack>(MsgPack_UnpackArray(this, index));
	}
	public MsgUnpack UnpackKey(int index) {
		return view_as<MsgUnpack>(MsgPack_UnpackKey(this, index));
	}
	public MsgUnpack UnpackValue(int index) {
		return view_as<MsgUnpack>(MsgPack_UnpackValue(this, index));
	}
	public MsgUnpack UnpackNext() {
		return view_as<MsgUnpack>(MsgPack_UnpackNext(this));
	}
	public any UnpackArrayCell(int index) {
		return MsgPack_UnpackArrayCell(this, index);
	}
	public void Print() {
		MsgPack_UnpackPrint(this);
	}
}