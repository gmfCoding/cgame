define print_data_and_len
  # $arg0 is the first argument; cast to pointer-to-struct if needed
  set $obj = $arg0
  p *($obj).data @ ($obj)._len
end
document print_data_and_len
Print bytes from obj.data for obj._len elements.
Usage: print_data_and_len <object-expression>
Example: print_data_and_len my_var
end
alias pstc = print_data_and_len