#include "pch.h"
#include "CommonType.h"

#include "Core/TokenContext.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

std::unordered_map<std::string, TypeAttributes> recognizedAttributes = {
	{ "const", TypeAttribute_Const },
	{ "unsigned", TypeAttribute_Unsigned },
	{ "signed", TypeAttribute_Signed },
	{ "volatile", TypeAttribute_Volatile },
};

CommonTypeBehavior::CommonTypeBehavior(size_t typeSize, const char* typeNamePtr)
	: mTypeSize(typeSize), mTypeNamePtr(typeNamePtr), mAttributes(TypeAttribute_None)
{
}

void CommonTypeBehavior::handle(const ReflectContext& context)
{
	/* Get attributes for the used type */
	mAttributes = context.tokenContext->getTypeAttributes();

	/* Clear list of attributes for the next type */
	context.tokenContext->clearTypeAttributes();

	std::string labelToken = "";
	const auto& secondToken = context.tokenContext->getToken(2);

	const bool isFunction = secondToken == "(" && context.scopeManager->getCurrentScope()->type != ScopeType_Unknown;
	const bool isVariable = (secondToken == ";" || secondToken == "=");
	const bool isArgument = context.scopeManager->isArgumentList();

	const bool isMember = !isArgument &&
		(context.scopeManager->hasScopeType(ScopeType_Struct) ||
		context.scopeManager->hasScopeType(ScopeType_Class));

	if (isArgument)
	{
		auto& argList = context.scopeManager->getArgumentListContext();

		argList.mNumArguments++;
	}
	else
	{
		labelToken = context.tokenContext->getToken(1);
	}

	if (isMember)
	{
		if (isVariable)
		{
			auto nameSpace = context.getCurrentNamespace();
			auto& lastType = nameSpace->mChildTypes.back();


			FieldDesc fieldDesc;
			fieldDesc.fieldName = labelToken;
			fieldDesc.size = mTypeSize;

			if (!lastType.fields.empty())
			{
				const auto& lastField = lastType.fields.back();

				fieldDesc.offset = lastField.offset + lastField.size;
			}
			else
			{
				fieldDesc.offset = 0;
			}

			lastType.fields.push_back(fieldDesc);

			lastType.typeSize += mTypeSize;
		}
	}

	if (isFunction)
	{
		context.descriptorContext->beginPendingFunction();
		auto& funcDesc = context.descriptorContext->getPendingFunctionDesc();

		funcDesc.name = labelToken;
		funcDesc.numArguments = 0;

		context.scopeManager->disableIgnoreScope();
	}
	else if (isArgument && context.descriptorContext->isPendingFunction())
	{
		auto& funcDesc = context.descriptorContext->getPendingFunctionDesc();

		funcDesc.numArguments++;
	}

#if 1 /* Debug print */
	printf(
		"Type Detected: [ Name: \"%s\", Label: \"%s\", Size: %llx, Attributes: %x, IsVariable: %s, IsArgument: %s, IsFunction: %s, IsMember: %s ]\n",
		mTypeNamePtr,
		labelToken.c_str(),
		mTypeSize,
		mAttributes,
		isVariable ? "Yes" : "No",
		isArgument ? "Yes" : "No",
		isFunction ? "Yes" : "No",
		isMember ? "Yes" : "No"
	);
#endif
}

size_t CommonTypeBehavior::getTypeSize() const
{
	return mTypeSize;
}

void CommonTypeBehavior::setAttributes(uint32_t attributes)
{
	mAttributes = attributes;
}

uint32_t CommonTypeBehavior::getAttributes() const
{
	return mAttributes;
}