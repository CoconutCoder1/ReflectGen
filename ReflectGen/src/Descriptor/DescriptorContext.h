#pragma once

#include "Type.h"
#include "Namespace.h"

class DescriptorContext
{
public:
	DescriptorContext()
	{
		mGlobalNamespace = new NamespaceDesc("GlobalNamespace", nullptr);
	}

	~DescriptorContext()
	{
		if (mGlobalNamespace)
		{
			delete mGlobalNamespace;
			mGlobalNamespace = nullptr;
		}
	}

	NamespaceDesc* getGlobalNamespace()
	{
		return mGlobalNamespace;
	}

	void outputToFile(std::ofstream& fileStream) noexcept;

private:
	void forwardDeclareTypes(std::ofstream& fileStream, const NamespaceDesc* namespaceDescPtr) noexcept;
	void declareTypeInfo(std::ofstream& fileStream, const NamespaceDesc* namespaceDescPtr) noexcept;

private:
	NamespaceDesc* mGlobalNamespace;
};