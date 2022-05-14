#include "dbc.hpp"

namespace dbc {

  assume_struct assume;
  guarantee_struct guarantee;
  pre_struct pre;
  post_struct post;
  loop_invariant_struct loop_invariant;

  invariant dbc_invariant_true("default");
  pre_post_true dbc_pre_post_true;
  assume_guarantee_true dbc_assume_guarantee_true;
  loop_true dbc_loop_true;
}
