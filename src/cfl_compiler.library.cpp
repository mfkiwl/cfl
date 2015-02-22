#include "cfl_compiler.h"

namespace Cfl {

llvm::Value* Compiler::create_random_function_struct(
        llvm::Function* parent,
        llvm::BasicBlock* entry_block)
{
    llvm::Function* cfl_rand_def =
        llvm::cast<llvm::Function>(top_module->getFunction("__cfl_rand"));

    if(!cfl_rand_def)
    {
        std::vector<llvm::Type*> time_args;
        time_args.push_back(builder->getInt32Ty());
        llvm::ArrayRef<llvm::Type*> time_args_ref(time_args);

        llvm::FunctionType* time_type = 
            llvm::FunctionType::get(builder->getInt32Ty(), time_args_ref, false);

        llvm::Constant* time_def = top_module->getOrInsertFunction("time", time_type);

        std::vector<llvm::Type*> srand_args;
        srand_args.push_back(builder->getInt32Ty());
        llvm::ArrayRef<llvm::Type*> srand_args_ref(srand_args);

        llvm::FunctionType* srand_type = 
            llvm::FunctionType::get(builder->getVoidTy(), srand_args_ref, false);

        llvm::Constant* srand_def = top_module->getOrInsertFunction("srand", srand_type);

        llvm::BasicBlock* main_block =
            top_module->getFunction("main")->begin()++;

        llvm::Instruction* first_instruction = main_block->begin()++;

        llvm::CallInst* time_call =
            llvm::CallInst::Create(time_def, builder->getInt32(0));

        main_block->getInstList().insert(first_instruction, time_call);

        llvm::CallInst* srand_call =
            llvm::CallInst::Create(srand_def, time_call);

        main_block->getInstList().insert(first_instruction, srand_call);

        std::vector<llvm::Type*> rand_args;
        llvm::ArrayRef<llvm::Type*> rand_args_ref(rand_args);

        llvm::FunctionType* rand_type = 
            llvm::FunctionType::get(builder->getInt32Ty(), rand_args_ref, false);

        llvm::Constant* rand_def = top_module->getOrInsertFunction("rand", rand_type);

        std::vector<llvm::Type*> cfl_rand_args;
        cfl_rand_args.push_back(builder->getInt32Ty());
        llvm::ArrayRef<llvm::Type*> cfl_rand_args_ref(cfl_rand_args);

        llvm::FunctionType* cfl_rand_type = 
            llvm::FunctionType::get(builder->getInt32Ty(), cfl_rand_args_ref, false);

        cfl_rand_def = llvm::Function::Create(
            cfl_rand_type, llvm::Function::ExternalLinkage, "__cfl_rand", top_module);

        llvm::BasicBlock* function_entry = llvm::BasicBlock::Create(
            global_context, "function_entry", cfl_rand_def);

        builder->SetInsertPoint(function_entry);

        llvm::Value* random_number = builder->CreateCall(
            rand_def, "random_number");

        llvm::Value* successor = builder->CreateAdd(
            cfl_rand_def->arg_begin()++, builder->getInt32(1), "successor");

        llvm::Value* result =
            builder->CreateSRem(random_number, successor, "result");

        builder->CreateRet(result);

        builder->SetInsertPoint(entry_block);
    }

    llvm::ArrayType* array_type =
        llvm::ArrayType::get(builder->getInt8PtrTy(), 0);

    std::vector<llvm::Type*> members;
    members.push_back(cfl_rand_def->getType());
    members.push_back(array_type->getPointerTo());
    llvm::ArrayRef<llvm::Type*> members_ref(members);

    llvm::StructType* struct_type =
        llvm::StructType::get(global_context, members_ref);

    std::vector<llvm::Constant*> initial_values;
    llvm::ArrayRef<llvm::Constant*> initial_values_ref(initial_values);

    llvm::Constant* initial_arguments =
        llvm::ConstantArray::get(array_type, initial_values_ref);

    llvm::Constant* initial_array_pointer =
        llvm::ConstantPointerNull::get(array_type->getPointerTo());

    std::vector<llvm::Constant*> function_values;
    function_values.push_back(cfl_rand_def);
    function_values.push_back(initial_array_pointer);
    llvm::ArrayRef<llvm::Constant*> function_values_ref(function_values);

    llvm::Constant* initial_struct =
        llvm::ConstantStruct::get(struct_type, function_values_ref);

    llvm::Value* arguments_pointer = call_malloc(array_type, parent, entry_block);

    llvm::Value* arguments_space = builder->CreatePointerCast(
        arguments_pointer, array_type->getPointerTo(), "arguments_space");

    builder->CreateStore(initial_arguments, arguments_space);

    return builder->CreateInsertValue(
        initial_struct, arguments_space, 1, "random_function_struct");
}

} // end namespace Cfl
