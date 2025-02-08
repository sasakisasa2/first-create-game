//
// DontDestroyOnLoad.h
//

#pragma once

#define DontDestroy DontDestroyOnLoad::GetInstance()

// Include Files.


class DontDestroyOnLoad final
{
private:
	DontDestroyOnLoad() = default;

public:
	~DontDestroyOnLoad() = default;

	DontDestroyOnLoad(DontDestroyOnLoad&&) = default;
	DontDestroyOnLoad& operator= (DontDestroyOnLoad&&) = default;

	DontDestroyOnLoad(DontDestroyOnLoad const&) = delete;
	DontDestroyOnLoad& operator= (DontDestroyOnLoad const&) = delete;

	static DontDestroyOnLoad* GetInstance()
	{
		static DontDestroyOnLoad instance;
		return &instance;
	}

private:



public:
	int attackAttribute = 2;
};
