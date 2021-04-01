//===--- ESRecordGetCheck.cpp - clang-tidy--------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ESRecordGetCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>

using namespace clang::ast_matchers;
namespace clang {
namespace tidy {
namespace cms {


void ESRecordGetCheck::registerMatchers(MatchFinder *Finder) {

  auto edmESGetToken = cxxRecordDecl(hasName("edm::ESGetToken"));

  auto ESRecord = cxxRecordDecl(
                            isSameOrDerivedFrom(hasName("EventSetupRecord"))
                          );

  auto ESRgetDecl = cxxMethodDecl(
                           hasName("get"),
                           ofClass(ESRecord)
                         );

  auto getCall = cxxMemberCallExpr(
                          callee(ESRgetDecl),
                          unless(hasArgument(0,expr(hasType(edmESGetToken))))
                        ).bind("getcallexpr");

  Finder->addMatcher(getCall,this);
}

void ESRecordGetCheck::report(CXXMemberCallExpr const * matchedCallExpr, calltype ct) {
  if (matchedCallExpr){
    clang::LangOptions LangOpts;
    LangOpts.CPlusPlus = true;
    clang::PrintingPolicy Policy(LangOpts);
    std::string replacement;
    auto callstart = matchedCallExpr->getBeginLoc();
    auto callend = matchedCallExpr->getEndLoc();
    std::string bufferi;
    llvm::raw_string_ostream outputi(bufferi);
    matchedCallExpr->printPretty(outputi,0,Policy);
    replacement=outputi.str();
    switch (ct) {
      case direct : {
        auto callrange = SourceRange(callstart,callend);
        diag(callstart, StringRef("direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit"), DiagnosticIDs::Warning);
//          << FixItHint::CreateReplacement(callrange, StringRef(replacement));
        break;
      };
    }
  }
}

void ESRecordGetCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *matchedCallExpr = Result.Nodes.getNodeAs<CXXMemberCallExpr>("getcallexpr")) {
         report(matchedCallExpr,direct);
  }
}


} // namespace cms
} // namespace tidy
} // namespace clang
