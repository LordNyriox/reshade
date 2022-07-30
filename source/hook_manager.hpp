/*
 * Copyright (C) 2014 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause OR MIT
 */

#pragma once

#include "hook.hpp"
#include <filesystem>
#include <type_traits>

#define HOOK_EXPORT extern "C"

template <typename T>
inline reshade::hook::address *vtable_from_instance(T *instance)
{
	static_assert(std::is_polymorphic<T>::value, "can only get virtual function table from polymorphic types");

	return *reinterpret_cast<reshade::hook::address **>(instance);
}

namespace reshade::hooks
{
	/// <summary>
	/// Installs a hook for the specified target function.
	/// </summary>
	/// <param name="name">Function name. This is used for logging and debugging only.</param>
	/// <param name="target">Address of the target function.</param>
	/// <param name="replacement">Address of the hook function.</param>
	/// <param name="queue_enable">Set this to <see langword="true"/> to queue the enable action instead of immediately executing it.</param>
	/// <returns>Status of the hook installation.</returns>
	bool install(const char *name, hook::address target, hook::address replacement, bool queue_enable = false);
	/// <summary>
	/// Installs a hook for the specified virtual function table entry.
	/// </summary>
	/// <param name="name">Function name. This is used for logging and debugging only.</param>
	/// <param name="vtable">Virtual function table pointer of the target object.</param>
	/// <param name="offset">Index of the target function in the virtual function table.</param>
	/// <param name="replacement">Address of the hook function.</param>
	/// <returns>Status of the hook installation.</returns>
	bool install(const char *name, hook::address vtable[], unsigned int offset, hook::address replacement);
	/// <summary>
	/// Uninstalls all previously installed hooks.
	/// Only call this function while the loader lock is held, since it is not thread-safe.
	/// </summary>
	void uninstall();

	/// <summary>
	/// Registers any matching exports in the specified module and installs or delays their hooking.
	/// Only call this function while the loader lock is held, since it is not thread-safe.
	/// </summary>
	/// <param name="path">File path to the target module.</param>
	void register_module(const std::filesystem::path &path);

	/// <summary>
	/// Loads the module for export hooks if it has not been loaded yet.
	/// </summary>
	void ensure_export_module_loaded();

	/// <summary>
	/// Checks whether the specified function is currently hooked.
	/// </summary>
	/// <param name="target">Original target address of a function.</param>
	bool is_hooked(hook::address target);

	/// <summary>
	/// Calls the original/trampoline function for the specified hook.
	/// </summary>
	/// <param name="target">Original target address of the hooked function (optional).</param>
	/// <param name="replacement">Address of the hook function.</param>
	/// <returns>Address of original/trampoline function.</returns>
	hook::address call(hook::address replacement, hook::address target);
	template <typename T>
	inline T call(T replacement, hook::address target = nullptr)
	{
		return reinterpret_cast<T>(call(reinterpret_cast<hook::address>(replacement), target));
	}
}
