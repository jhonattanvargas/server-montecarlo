'use strict'

//add models require
const operator = require('suma-c-jhonattan-1').addon

//add functions from routes
function add(req,res){
  let a = parseFloat(req.params.a)
  let b = parseFloat(req.params.b)
  let result = operator.add(a,b)
  if(isNaN(result)){
    res
      .status(500)
      .send({message:'Error al operar la suma'})
  }
  else{
    res
    .status(200)
    .send({result})
  }
}

function sub(req,res){
  let a = parseFloat(req.params.a)
  let b = parseFloat(req.params.b)
  let result = operator.sub(a,b)
  if(isNaN(result)){
    res
      .status(500)
      .send({message:'Error al operar la resta'})
  }
  else{
    res
    .status(200)
    .send({result})
  }
}

function mul(req,res){
  let a = parseFloat(req.params.a)
  let b = parseFloat(req.params.b)
  let result = operator.mul(a,b)
  if(isNaN(result)){
    res
      .status(500)
      .send({message:'Error al operar la multiplicación'})
  }
  else{
    res
    .status(200)
    .send({result})
  }
}

function div(req,res){
  let a = parseFloat(req.params.a)
  let b = parseFloat(req.params.b)
  let result = operator.div(a,b)
  if(isNaN(result)){
    res
      .status(500)
      .send({message:'Error al operar la división'})
  }
  else{
    res
    .status(200)
    .send({result})
  }
}

module.exports = {
  add,
  sub,
  mul,
  div
}
