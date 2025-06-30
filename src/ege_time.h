#pragma once

namespace ege
{
struct _graph_setting;

double get_highfeq_time_ls(struct _graph_setting* pg);

void updateFrameRate(bool addFrameCount = true);

} // namespace ege
