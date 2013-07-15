import sublime_plugin
import re


class MySubstituteCommand(sublime_plugin.TextCommand):
    """Do all regex replacements in 'from' to 'to' in the current selection"""
    def run(self, edit, **args):
        for region in self.view.sel():
            cur = self.view.substr(region)
            src = args['from']
            dest = args['to']
            for i in range(0, len(src)):
                cur = re.sub(src[i], dest[i], cur, re.S)
            self.view.replace(edit, region, cur)
