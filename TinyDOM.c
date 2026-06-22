#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TinyDOM.h"
void TinyDOM_SetAttribute(Tag *tag, char *attr, char *val) {
	for (int i = 0; i < tag->AttributeCount; i++) {
		if (strcmp(tag->attributes[i].attrib, attr) == 0) {
			tag->attributes[i].val = val;
			return;
		}
	}
	tag->AttributeCount++;
	tag->attributes = realloc(tag->attributes, tag->AttributeCount*sizeof(Attribute));
	tag->attributes[tag->AttributeCount - 1].attrib = attr;
	tag->attributes[tag->AttributeCount - 1].val = val;
}
char* TinyDOM_GetAttribute(Tag *tag, char *attr) {
	for (int i = 0; i < tag->AttributeCount; i++) {
		if (strcmp(tag->attributes[i].attrib, attr) == 0) {
			return tag->attributes[i].val;
		}
	}
	return NULL;
}
char *TinyDOM_ExportAttribs(Tag *tag) {
	int str_len = 0;
	char *str = NULL;
	for (int i = 0; i < tag->AttributeCount; i++) {
		int trailingSpace = i < (tag->AttributeCount - 1);
		int oldstrlen = str_len;
		str_len += strlen(tag->attributes[i].attrib) + 3 +
                   strlen(tag->attributes[i].val) + trailingSpace;
		str = realloc(str, str_len + 1);
		if (trailingSpace) {
			sprintf(str + oldstrlen, "%s=\"%s\" ", tag->attributes[i].attrib,
					tag->attributes[i].val);
		} else {
			sprintf(str + oldstrlen, "%s=\"%s\"", tag->attributes[i].attrib,
					tag->attributes[i].val);
		}
	}
	str[str_len] = '\0';
	return str;
}
Tag *TinyDOM_CreateTag(Tag *Parent, char *tag) {
	if (!Parent) {
		Tag *res = malloc(sizeof(Tag));
		res->isTag = 1;
		res->tag = tag;
		res->AttributeCount = 0;
		res->attributes = NULL;
		res->children = NULL;
		res->ChildrenCount = 0;
		return res;
	} else {
		Tag *res = malloc(sizeof(Tag));
		res->isTag = 1;
		res->tag = tag;
		res->AttributeCount = 0;
		res->attributes = NULL;
		res->children = NULL;
		res->ChildrenCount = 0;

		Parent->ChildrenCount++;
		Parent->children =
			realloc(Parent->children, Parent->ChildrenCount * sizeof(Tag*));
		Parent->children[Parent->ChildrenCount - 1] = res;
		return Parent->children[Parent->ChildrenCount - 1];
	}
}
Tag *TinyDOM_CreateTextTag(Tag *Parent, char *tag) {
	// this isnt meant to be used as root so no check for parent, crash it
	// folks!

	Tag *res = malloc(sizeof(Tag));
	res->isTag = 0;
	res->tag = tag;
	res->AttributeCount = 0;
	res->attributes = NULL;
	res->children = NULL;
	res->ChildrenCount = 0;

	Parent->ChildrenCount++;
	Parent->children =
		realloc(Parent->children, Parent->ChildrenCount * sizeof(Tag*));
	Parent->children[Parent->ChildrenCount - 1] = res;
	return Parent->children[Parent->ChildrenCount - 1];
}
char *TinyDOM_ExportHTML(Tag *root) {
	if (root->children) {
		char *inside = NULL;
		int csoi = 0;
		for (int i = 0; i < root->ChildrenCount; i++) {
			char *tis = TinyDOM_ExportHTML(root->children[i]);
			int len = strlen(tis);
			inside = realloc(inside, csoi + len + 1);
			strcpy(inside + csoi, tis);
			csoi += len;
			free(tis);
		}
		inside[csoi]='\0';
		if (root->AttributeCount) {
			char *renderedattrib = TinyDOM_ExportAttribs(root);
			char *ret = malloc(strlen(root->tag) * 2 + strlen(inside) + 1 + 1 +
							   strlen(renderedattrib)+5);
			sprintf(ret, "<%s %s>%s</%s>", root->tag, renderedattrib, inside,
					root->tag);
			free(renderedattrib);
			return ret;
		} else {
			char *ret = malloc(strlen(root->tag) * 2 + strlen(inside) + 1+5);
			sprintf(ret, "<%s>%s</%s>", root->tag, inside, root->tag);
			return ret;
		}
	} else {
		if (root->isTag) {
			if (root->AttributeCount) {
				char *renderedattrib = TinyDOM_ExportAttribs(root);
				char *ret = malloc(strlen(root->tag) * 2 + 1 + 1 +
								   strlen(renderedattrib)+5);
				sprintf(ret, "<%s %s></%s>", root->tag, renderedattrib,
						root->tag);
				free(renderedattrib);
				return ret;
			} else {
				char *ret = malloc(6 + strlen(root->tag) * 2);
				sprintf(ret, "<%s></%s>", root->tag, root->tag);
				return ret;
			}
		} else {
			return strdup(root->tag);
		}
	}
}



// HELPERS
Tag *TinyDOM_CreateParagraph(Tag* Parent, char* inside) {
	Tag *tag = TinyDOM_CreateTag(Parent, "p");
	TinyDOM_CreateTextTag(tag, inside);
	return tag;
}