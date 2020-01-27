#include "MceditBackend.h"
#include "TestTool.h"


int main2()
{
	TestTool *tool = new TestTool();
	BaseBackend *backend = new MceditBackend(tool);

	backend->bind(".File.New", []() {}, "");
	backend->bind(".File.New2", []() {}, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa");
	backend->bind(".File2.New", []() {}, "");
	backend->bind(".File3.New", []() {}, "");
	backend->bind(".File4.New", []() {}, "");
	backend->bind(".File5.New", []() {}, "");
	backend->bind(".Test.Fill", [&tool]() { tool->fill(); }, "");
	backend->bind(".Test.Unsave", [&tool]() {tool->unsave(); }, "");
	backend->bind(".Test.Param${Cos tam ciekawego o parameeeeeeeeeeeeeeeeeeeeetrzeeeeeeee adadhdhsahdsadasdhsahdhashdhsahdhashdhahds:|param}", [&tool]() {tool->param(); }, "");
	backend->bind("#mcedit#<EDITION>", [&tool]() { tool->edition(); }, "");
	backend->bind("#mcedit#.Test.Clear", [&tool]() {tool->clear(); }, "Clear tool");

	backend->start();

	delete backend;
	delete tool;
	return 0;
}