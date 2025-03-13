#include <ruby.h>
#include <ruby/encoding.h>
#include <ruby/re.h>
#include <ruby/regint.h>
#include <pcre.h>

// Function declarations
static VALUE parse_address_with_regexp(VALUE self, VALUE address_str, VALUE regexp_str);
static VALUE match_to_hash_ext(VALUE self, VALUE match_data);

// Initialization function for the module
void Init_street_address_ext(void) {
    VALUE mStreetAddress = rb_define_module("StreetAddress");
    VALUE cUS = rb_define_class_under(mStreetAddress, "US", rb_cObject);
    VALUE mRegexHelper = rb_define_module_under(cUS, "RegexHelper");
    
    rb_define_singleton_method(mRegexHelper, "parse_address_with_regexp", parse_address_with_regexp, 2);
    rb_define_singleton_method(mRegexHelper, "match_to_hash_ext", match_to_hash_ext, 1);
}

/*
 * Optimized function to perform regex match and extract the named captures
 * into a Ruby hash. This is the main performance bottleneck in the original code.
 */
static VALUE parse_address_with_regexp(VALUE self, VALUE address_str, VALUE regexp_str) {
    Check_Type(address_str, T_STRING);
    Check_Type(regexp_str, T_STRING);
    
    // Create the regex object
    VALUE regexp = rb_reg_new(RSTRING_PTR(regexp_str), RSTRING_LEN(regexp_str), 0);
    
    // Perform the match
    VALUE match_result = rb_reg_match(regexp, address_str);
    
    if (NIL_P(match_result)) {
        return Qnil;
    }
    
    // Get the MatchData object
    VALUE match_data = rb_backref_get();
    
    // Return the match data for further processing
    return match_data;
}

/*
 * Optimized version of the match_to_hash method which extracts 
 * named captures from a MatchData object into a hash.
 */
static VALUE match_to_hash_ext(VALUE self, VALUE match_data) {
    if (NIL_P(match_data) || rb_obj_class(match_data) != rb_cMatchData) {
        return Qnil;
    }
    
    VALUE hash = rb_hash_new();
    VALUE names = rb_funcall(match_data, rb_intern("names"), 0);
    long len = RARRAY_LEN(names);
    
    for (long i = 0; i < len; i++) {
        VALUE name = rb_ary_entry(names, i);
        VALUE matched_str = rb_funcall(match_data, rb_intern("[]"), 1, name);
        
        if (!NIL_P(matched_str)) {
            rb_hash_aset(hash, name, matched_str);
        }
    }
    
    return hash;
}