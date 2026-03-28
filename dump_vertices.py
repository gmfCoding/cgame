# # dump_vertices.py
# import lldb, json

# def format_vec3(val):
#     # expect struct with fields x,y,z (or nested union v)
#     try:
#         x = val.GetChildMemberWithName('x').GetValueAsSigned()
#         y = val.GetChildMemberWithName('y').GetValueAsSigned()
#         z = val.GetChildMemberWithName('z').GetValueAsSigned()
#         return [x,y,z]
#     except:
#         # fallback: try floats
#         try:
#             x = val.GetChildMemberWithName('x').GetValue()
#             y = val.GetChildMemberWithName('y').GetValue()
#             z = val.GetChildMemberWithName('z').GetValue()
#             return [float(x), float(y), float(z)]
#         except:
#             # try union member 'v' array of 3
#             v = val.GetChildMemberWithName('v')
#             if v.IsValid():
#                 return [float(v.GetChildAtIndex(i).GetValue()) for i in range(3)]
#     return None

# def format_vec2(val):
#     try:
#         u = val.GetChildMemberWithName('x').GetValue()
#         v = val.GetChildMemberWithName('y').GetValue()
#         return [float(u), float(v)]
#     except:
#         # try array 'v'
#         arr = val.GetChildMemberWithName('v')
#         if arr.IsValid():
#             return [float(arr.GetChildAtIndex(0).GetValue()), float(arr.GetChildAtIndex(1).GetValue())]
#     return None

# def dump_vertices(debugger, command, result, internal_dict):
#     # usage: dump_vertices <array-value> [count]
#     args = command.split()
#     if not args:
#         print("usage: dump_vertices <array-value> [count]")
#         return
#     target = debugger.GetSelectedTarget()
#     frame = target.GetProcess().GetSelectedThread().GetSelectedFrame()
#     expr = args[0]
#     count = int(args[1]) if len(args) > 1 else None

#     # evaluate count from .size/_len if not provided
#     if count is None:
#         try:
#             size_val = frame.EvaluateExpression(f"{expr}.size")
#             if not size_val.GetError().Fail():
#                 count = int(size_val.GetValue())
#             else:
#                 size_val = frame.EvaluateExpression(f"{expr}._len")
#                 count = int(size_val.GetValue()) if not size_val.GetError().Fail() else 0
#         except:
#             count = 0

#     arr_expr = f"((GPUVertex *){expr}.data)[0]@{count}"
#     val = frame.EvaluateExpression(arr_expr)
#     if val.GetError().Fail():
#         print("failed to evaluate:", arr_expr)
#         return

#     out = []
#     for i in range(count):
#         v = val.GetChildAtIndex(i)
#         pos = format_vec3(v.GetChildMemberWithName('position'))
#         tex = format_vec2(v.GetChildMemberWithName('texcoord'))
#         # skip union internals: only show 'normal' as numeric triplet but avoid union subfields
#         norm = format_vec3(v.GetChildMemberWithName('normal'))
#         out.append({'index': i, 'position': pos, 'texcoord': tex, 'normal': norm})

#     print(json.dumps(out, indent=2))

# def __lldb_init_module(debugger, internal_dict):
#     debugger.HandleCommand('command script add -f dump_vertices.dump_vertices dump_vertices')
#     print('The "dump_vertices" command has been installed. Usage: dump_vertices <array> [count]')
# dump_vertices_gdb.py
import gdb, json

def read_child_float(valobj, name):
    try:
        child = valobj[name]
        return float(child.cast(gdb.lookup_type('float')))
    except Exception:
        try:
            # array-like child (e.g., v[0])
            arr = valobj[name]
            return float(arr[0])
        except Exception:
            return None

def format_vec3(valobj):
    # prefer x,y,z; fallback to v[0..2]
    try:
        x = read_child_float(valobj, 'x')
        y = read_child_float(valobj, 'y')
        z = read_child_float(valobj, 'z')
        if None not in (x,y,z):
            return [x,y,z]
    except Exception:
        pass
    try:
        v = valobj['v']
        return [float(v[0]), float(v[1]), float(v[2])]
    except Exception:
        return None

def format_vec2(valobj):
    try:
        x = read_child_float(valobj, 'x')
        y = read_child_float(valobj, 'y')
        if None not in (x,y):
            return [x,y]
    except Exception:
        pass
    try:
        v = valobj['v']
        return [float(v[0]), float(v[1])]
    except Exception:
        return None

class DumpVerticesCommand(gdb.Command):
    """Dump GPUVertex array: dump_vertices <expr> [count]
    <expr> should be a struct with .data (pointer) and .size or ._len
    """

    def __init__(self):
        super(DumpVerticesCommand, self).__init__('dump_vertices', gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        args = gdb.string_to_argv(arg)
        if not args:
            print("usage: dump_vertices <array> [count]")
            return
        expr = args[0]
        count = None
        if len(args) > 1:
            count = int(args[1])

        if count is None:
            # try .size then ._len
            try:
                cnt_val = gdb.parse_and_eval(f"{expr}.size")
                count = int(cnt_val)
            except Exception:
                try:
                    cnt_val = gdb.parse_and_eval(f"{expr}._len")
                    count = int(cnt_val)
                except Exception:
                    print("failed to determine count; provide it explicitly")
                    return

        # evaluate array pointer
        try:
            ptr = gdb.parse_and_eval(f"(GPUVertex *)({expr}.data)")
        except Exception as e:
            print("failed to evaluate pointer:", e)
            return

        out = []
        for i in range(count):
            try:
                elem = (ptr + i).dereference()
            except Exception as e:
                print(f"failed to read element {i}: {e}")
                break
            pos = format_vec3(elem['position'])
            tex = format_vec2(elem['texcoord'])
            norm = format_vec3(elem['normal'])
            out.append({'index': i, 'position': pos, 'texcoord': tex, 'normal': norm})

        print(json.dumps(out, indent=2))

DumpVerticesCommand()
