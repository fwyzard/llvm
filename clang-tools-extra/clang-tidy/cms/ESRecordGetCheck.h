//===--- ESRecordGetCheck.h - clang-tidy------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CMS_ESRGET_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CMS_ESRGET_H

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"
#include <llvm/Support/SaveAndRestore.h>

namespace clang {
namespace tidy {
namespace cms {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/esrget.html
class ESRecordGetCheck : public ClangTidyCheck {
public:
  ESRecordGetCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  enum calltype {direct};
  void report(CXXMemberCallExpr const * matchedCallExpr, calltype);

};

} // namespace cms
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CMS_ESRGET_H
