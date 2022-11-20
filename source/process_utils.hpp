/*
 * Copyright (C) 2022 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <filesystem>

namespace reshade::utils
{
	/// <summary>
	/// Opens a Windows explorer window with the specified file selected.
	/// </summary>
	bool open_explorer(const std::filesystem::path &path);

	/// <summary>
	/// Executes the specified command as a new process, with basic (not elevated) user privileges.
	/// </summary>
	bool execute_command(const std::string &command_line, const std::filesystem::path &working_directory = L".", bool no_window = false);
}
