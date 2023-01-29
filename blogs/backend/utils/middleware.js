const User = require('../models/user')
const jwt = require('jsonwebtoken')

const unknownEndpoint = (request, response) => {
    response.status(404).send({ error: 'unknown endpoint' })
  }

const userExtractor = async (request, response, next) => {
    const token = request.token
    let decodedToken;
    try {
        decodedToken = jwt.verify(token, process.env.SECRET)
    }
    catch(exception){
        return next(exception)
    }
    if (!token || !decodedToken || !decodedToken.id){
      return response.status(401).json({error: 'token missing or invalid'})
    }
    request.user = await User.findById(decodedToken.id)
    next()
}

const tokenExtractor = (request, response, next) => {
    const authorization = request.get('authorization')
    if (authorization && authorization.toLowerCase().startsWith('bearer ')) {
      request.token = authorization.substring(7)
    }
    else {
        request.token = null
    }
    next()
}

const errorHandler = (error, request, response, next) => {
  
    if (error.name === 'CastError') {
      return response.status(400).send({ error: 'malformatted id' })
    } else if (error.name === 'ValidationError') {
      return response.status(400).json({ error: error.message })
    }
    else if (error.name === 'JsonWebTokenError') {
        return response.status(401).json({
          error: 'invalid token'
        })
    }
    next(error)
  }


module.exports = {
    unknownEndpoint,
    errorHandler,
    tokenExtractor,
    userExtractor
}