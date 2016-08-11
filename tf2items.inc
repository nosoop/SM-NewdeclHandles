#if defined _tf2items_newdecls_included
	#endinput
#endif

#define _tf2items_newdecls_included

#if !defined _tf2items_included
	#pragma newdecls optional
	#include <tf2items>
#endif

#pragma newdecls required

methodmap TF2Item < Handle {
	public TF2Item(int flags) {
		return view_as<TF2Item>(TF2Items_CreateItem(flags));
	}
	
	property int Flags {
		public set(int flags) {
			TF2Items_SetFlags(this, flags);
		}
		public get() {
			TF2Items_GetFlags(this);
		}
	}
	
	public void SetClassname(char[] classname) {
		TF2Items_SetClassname(this, classname);
	}
	
	public void GetClassname(char[] buffer, int maxlen) {
		TF2Items_GetClassname(this, buffer, maxlen);
	}
	
	property int DefIndex {
		public set(int defindex) {
			TF2Items_SetItemIndex(this, defindex);
		}
		public get() {
			return TF2Items_GetItemIndex(this);
		}
	}
	
	property int Quality {
		public set(int quality) {
			TF2Items_SetQuality(this, quality);
		}
		public get() {
			return TF2Items_GetQuality(this);
		}
	}
	
	property int Level {
		public set(int level) {
			TF2Items_SetLevel(this, level);
		}
		public get() {
			return TF2Items_GetLevel(this);
		}
	}
	
	property int AttributeCount {
		public set(int count) {
			TF2Items_SetNumAttributes(this, count);
		}
		public get() {
			return TF2Items_GetNumAttributes(this);
		}
	}
	
	public void SetAttribute(int slot, int attribDefIndex, float flValue) {
		TF2Items_SetAttribute(this, slot, attribDefIndex, flValue);
	}
	
	public void GetAttribute(int slot, int &attribDefIndex = 0, float &flValue = 0.0) {
		attribDefIndex = TF2Items_GetAttributeId(this, slot);
		flValue = TF2Items_GetAttributeValue(this, slot);
	}
	
	public int Give(int client) {
		return TF2Items_GiveNamedItem(client, this);
	}
}