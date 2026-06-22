typedef struct Attribute Attribute;
typedef struct Tag Tag;

struct Attribute {
	char *attrib;
	char *val;
};

struct Tag {
	int isTag;
	char *tag;
	Attribute *attributes;
	int AttributeCount;
	Tag **children;
	int ChildrenCount;
};
char *TinyDOM_ExportAttribs(Tag *tag);
Tag *TinyDOM_CreateTag(Tag *Parent, char *tag);
Tag *TinyDOM_CreateTextTag(Tag *Parent, char *tag);
char *TinyDOM_ExportHTML(Tag *root);
void TinyDOM_SetAttribute(Tag *tag, char *attr, char *val);
char* TinyDOM_GetAttribute(Tag *tag, char *attr);