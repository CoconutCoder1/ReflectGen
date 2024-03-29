#pragma once

using TokenList = std::vector<std::string>;

static const std::string TokenContext_NullToken = "";

class TokenContext
{
public:
	TokenContext(TokenList&& tokens) noexcept
		: mTokens(tokens)
	{
		mCurrentIter = mTokens.begin();
	}

	const std::string& getToken(size_t offset) const noexcept
	{
		TokenList::const_iterator iter = mCurrentIter + offset;
		if (iter > mTokens.begin() && iter < mTokens.end())
			return *iter;

		return TokenContext_NullToken;
	}

	/* Advance the iterated token by a certain number of steps */
	void advance(size_t count) noexcept
	{
		while (mCurrentIter < mTokens.end() && count > 0)
		{
			mCurrentIter++;
			count--;
		}
	}

	bool isEnd() const
	{
		return !(mCurrentIter < mTokens.end());
	}

private:
	TokenList mTokens;
	TokenList::const_iterator mCurrentIter;
};